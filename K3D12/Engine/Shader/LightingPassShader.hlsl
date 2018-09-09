
#define TBDRRootSignature   "RootFlags(0),"\
                                    "CBV(b0),"\
                                    "DescriptorTable(SRV(t0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t2,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t3,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t4,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\

struct SurfaceData
{
    float3 posView;
    float3 normalView;
    float4 albedo;
    float specular;
};

struct LightParamater
{
    float3 color;
    float3 pos;
    float3 direction;
    float range;
    float attenuation;
    float intensity;
    int type; // 0 = point,1 = Direction
};

cbuffer CameraData : register(b0)
{
    matrix View;
    matrix Proj;
    float windowWidth;
    float windowHeight;
}

Texture2D<float4> NormalMapTexture : register(t0);
Texture2D<float4> SpecularPowerMapTexture : register(t1);
Texture2D<float4> AlbeboMapTexture : register(t2);
Texture2D<float> DepthTexture : register(t3);
StructuredBuffer<LightParamater> Lights : register(t4);

RWTexture2D<float4> ResultTexture : register(u0);

#define threadX  16
#define threadY  16
#define threadDimension 16
#define ComputeTileSize (threadX*threadY)
#define LightNumMax 1024                        //���C�g��1024�܂ŃT�|�[�g

groupshared uint sMinZ; //�^�C���̍ŏ��k�x
groupshared uint sMaxZ; //�^�C���̍ő�[�x
groupshared uint sTileLightIndices[LightNumMax]; //�^�C�����ڐG���Ă��郉�C�g�̃C���f�b�N�X
groupshared uint sTileNumLights;


void GetTileFrustumPlane(out float4 frustumPlanes[6], uint3 groupId , float screenWidth,float screenHeight)
{
	// �^�C���̍ő�E�ŏ��[�x�𕂓������_�ɕϊ�
    float minTileZ = asfloat(sMinZ);
    float maxTileZ = asfloat(sMaxZ);

    float width = screenWidth;
    float height = screenHeight;
    float2 screenSize = float2(width, height);

    float2 tileScale = screenSize * rcp(float(2 * threadDimension));
    float2 tileBias = tileScale - groupId.xy;

    float4 c1 = float4(Proj._11 * tileScale.x, 0.0, tileBias.x, 0.0);
    float4 c2 = float4(0.0, -Proj._22 * tileScale.y, tileBias.y, 0.0);
    float4 c4 = float4(0.0, 0.0, 1.0, 0.0);

    frustumPlanes[0] = c4 - c1; // �E����
    frustumPlanes[1] = c1; // ������
    frustumPlanes[2] = c4 - c2; // �㕽��
    frustumPlanes[3] = c2; // �ꕽ��
    //�j�A�E�t�@�[�̕���
    frustumPlanes[4] = float4(0.0, 0.0, 1.0, -minTileZ);

    frustumPlanes[5] = float4(0.0, 0.0, -1.0, maxTileZ);

	// �@�������K������Ă��Ȃ�4�ʂɂ��Ă������K������
	[unroll(4)]
    for (uint i = 0; i < 4; ++i)
    {
        frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
    }

}

//! �T�[�t�F�C�X�����擾����
SurfaceData GetSurfaceData(uint2 uv)
{
    float depth = DepthTexture[uv];
    float4 albedo = AlbeboMapTexture[uv];
    float4 specPower = SpecularPowerMapTexture[uv];

	// �r���[��Ԃł̍��W�����߂�
    float4 norm = (2.0f * NormalMapTexture.Load(int3(uv.x, uv.y, 0)))-1.0f; //�@���̃f�R�[�h
    
    float sqrZ = 0.0f;
    float sqrtValue = 1.0f - (norm.x * norm.x) - (norm.y * norm.y);
    sqrZ = sqrt(sqrtValue);
    float3 normal = float3(norm.x, norm.y, norm.z); //Z���x�N�g���̎Z�o
    float screenSopaceDepth = length(normal);
    //�[��
    screenSopaceDepth = Proj._34 / (depth - Proj._33);

    float2 gbufferDim = float2(0.0f, 0.0f);
    uint dummy = 0;
    uint dummy2 = 0;
    NormalMapTexture.GetDimensions(dummy2, gbufferDim.x, gbufferDim.y, dummy);

    float2 screenPixelOffset = (float2(2.0f, -2.0f)) / gbufferDim;
    float2 positionScreen = (float2(uv) + float2(0.5f, 0.5f)) * screenPixelOffset.xy + float2(-1.0f, 1.0f);
    float2 viewRay = positionScreen.xy / float2(Proj._11, Proj._22);
    float3 pos = (float3) 0;

    pos.z = screenSopaceDepth;
    pos.xy = viewRay.xy * screenSopaceDepth;

    SurfaceData ret =
    {
        pos,
	    normal,
	    albedo.xyzw,
		specPower.y
    };
    return ret;
}

//�|�C���g���C�g�̑o�������˗����z�֐�
float4 PointLightBRDF(float3 pos, float3 normal, float3 lightPos, float3 lightColor, float range, float lightAttenuation, float specularPower, float f0)
{
    float3 viewDir = -normalize(pos);

    //�����v�Z
    float3 lightDir = lightPos - pos;
    float lightLength = length(lightDir);
    float lightRatio = lightLength / (range);
    float attenuation = max(1.0f - (lightRatio * lightRatio), 0.0f);
    if (attenuation == 0.0f)
    {
       float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    lightDir = normalize(lightDir);
    //���C�g�v�Z
    float dotNL = (normal.x * lightDir.x) + (normal.y * lightDir.y) + (normal.z * lightDir.z);
    float nl = saturate(dotNL);
    float3 halfVec = normalize(viewDir + lightDir);
    float nh = saturate(dot(normal, halfVec));
    float power = lightAttenuation;
    float spec = 0.0f;
    if (power > 0.0f)
    {
        float nv = saturate(dot(normal, viewDir));
        float sn = pow(2.0f, 13.0f * specularPower) * power;
        float D = (sn + 2.0f) * pow(nh, sn) / (2.0f * 3.1415926f);
        float F = f0 + (1.0f- f0) * pow((1.0f - nv), 5.0f);
        float dv = saturate(dot(viewDir, (viewDir + lightDir)));
        float V = 4.0f / (dv * dv);
        float nlnv = nl * nv;
        float G = V * nlnv;

        spec = max(F * G * D / (4.0 * nlnv), 0.0);

    }

    return float4(lightColor.rgb * nl * attenuation, spec * attenuation * attenuation);

}

float4 CalcDirectionalLightBRDF(float3 objPos, float3 objNormal, float3 litDir, float3 litColor, float litPower, float gloss, float f0)
{
    float3 eyeVec = normalize(-objPos);
    litDir *= -1;

	// ���C�g�v�Z
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = saturate(dot(objNormal, halfVec));
    float power = litPower;
    float spec = 0.0f;
    if (power > 0.0f)
    {
        float nv = saturate(dot(objNormal, eyeVec));
        float sn = pow(2.0, 13.0 * gloss) * power;
        float D = (sn + 2.0)  * pow(nh, sn)/ (2.0 * 3.1415926);
        float F = f0 + (1.0 - f0) * pow((1.0 - nv), 5.0);
        float dv = saturate(dot(eyeVec, (eyeVec + litDir)));
        float V = 4.0 / (dv * dv);
        float nl_nv = nl * nv;
        float G = V * nl_nv;
        spec = max(F * G * D / (4.0 * nl_nv),0.0f);
    }

	// �o��
    return saturate(float4(litColor.rgb * nl, spec));
}


[RootSignature(TBDRRootSignature)]
[numthreads(threadX, threadY, 1)]
void CSMain(uint3 groupeID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint groupIndex = groupThreadID.y * threadDimension + groupThreadID.x;
    uint2 globalIndex = dispatchThreadID.xy;

    //���L�������̏�����
    if (groupIndex == 0)
    {
        sTileNumLights = 0;
        sMinZ = 0x7F7FFFFF; // float�̍ő�l
        sMaxZ = 0;
    }

    //�ŏ������i�v���W�F�N�V�����s�񂩂�擾�j
    float minZSample = -Proj._34 / (Proj._33 - 1.0f);
    //�ő勗���i�v���W�F�N�V�����s�񂩂�擾�j
    float maxZSample = -Proj._34 / Proj._33;

    //�T�[�t�F�X�i�T���v�����O�����e�N�Z���̏��j���擾
    SurfaceData surfaceData = GetSurfaceData(globalIndex);

    bool validPixel =
                 surfaceData.posView.z >= -Proj._34 / Proj._33 &&
                 surfaceData.posView.z <= -Proj._34 / (Proj._33 - 1.0f);
    [flatten]
    if (validPixel)
    {
        minZSample = min(minZSample, surfaceData.posView.z);
        maxZSample = max(maxZSample, surfaceData.posView.z);
    }

    //�r���[�X�y�[�X�ł̐[�x
    //����������
    GroupMemoryBarrierWithGroupSync();
    //��������

    if (maxZSample >= minZSample)
    {
        InterlockedMin(sMinZ, asuint(minZSample));
        InterlockedMax(sMaxZ, asuint(maxZSample));
    }

    //��������            
    GroupMemoryBarrierWithGroupSync();


    float2 gbufferDim = float2(0, 0);
    uint dummy = 0;
    uint dummy2 = 0;
    NormalMapTexture.GetDimensions(dummy2, gbufferDim.x, gbufferDim.y, dummy);

    float4 frustumPlanes[6];
    GetTileFrustumPlane(frustumPlanes, groupeID, gbufferDim.x, gbufferDim.y);
   
    //���C�g�̑�������уX�g���C�h���i�X�g���C�h�̓_�~�[�j
    //GetDimension�Ń��C�g�̐��ƃ��C�g�̃X�g���C�h�l���擾�B
    uint strid = 0;
    uint lightNum = 0;
    Lights.GetDimensions(lightNum, strid);
   
    //���C�g�J�����O
    {
        uint threadCount = ComputeTileSize;
        uint passCount = (lightNum + threadCount - 1) / threadCount;

        for (uint passIt = 0; passIt < passCount; ++passIt)
        {
            uint lightIndex = passIt * threadCount + groupIndex;
            lightIndex = min(lightIndex, lightNum);
            
            //���C�g�̎擾
            LightParamater light = Lights[lightIndex];

            //���C�g���J�������_�̃r���[���W�ɂ����Ă���
            light.pos = mul(View, float4(light.pos.xyz, 1.0)).xyz;

            //�t���X�^���Ƃ̓����蔻��̌���
            bool inFrustum = true;

            //���s���~����܂�For����W�J
            [unroll]
            for (uint i = 0; i < 6; ++i)
            {
                float4 lp = float4(light.pos.xyz, 1.0);
                float d = dot(frustumPlanes[i], lp);

                //�������s�������C�g�i�T�����C�g�j�Ȃ��
                if (light.type == 1)
                {
                    //��������OK
                    inFrustum = true;
                    break;
                }
                inFrustum = inFrustum && (d >= -light.range);
            }

            if (inFrustum)
            {
                uint offset = 0;
                InterlockedAdd(sTileNumLights, 1, offset);
                sTileLightIndices[offset] = lightIndex;
            }
        }
    }
    //�����ŃX���b�h�Ԃœ������Ƃ�̂ŁA�^�C���ƏՓ˂��Ă��郉�C�g�̌��o���\�ɂȂ��Ă���͂�
    GroupMemoryBarrierWithGroupSync();

    //���C�g�C���f�b�N�X���o�̓o�b�t�@�ɏo��
    //���C�e�B���O

    float3 diffuse = (float3) 0.0;
    float specular = 0.0f;
    float4 result;

    for (uint i = 0; i < sTileNumLights; ++i)
    {
        uint lightIndex = sTileLightIndices[i];
        LightParamater light = Lights[lightIndex];
        float attenuation = light.attenuation;
        if (surfaceData.specular <= 0.0f)
        {
            light.attenuation = 0.0f;
        }

        //�|�W�V�������J������Ԃɂ����Ă���
        float3 lightViewPos = mul(View, float4(light.pos.xyz, 1.0f)).xyz;
        //�x�N�g����view��Ԃɂ����Ă���
        float3 lightViewDirection = mul(View, float4(light.direction.xyz, 0.0f)).xyz;

        //�^�C�v�ŕ���@���Ȃ��낵���Ȃ�
        if (light.type == 0)
        {
            result = PointLightBRDF(surfaceData.posView, surfaceData.normalView, lightViewPos, light.color.xyz, light.range, light.attenuation, surfaceData.specular, 0.5f);
        }
        else
        {
            result = CalcDirectionalLightBRDF(surfaceData.posView, surfaceData.normalView, lightViewDirection, light.color, light.attenuation, surfaceData.specular, 0.5f);
        }
        diffuse = diffuse + result.rgb;
       
        specular = specular + result.a;

    }

    //�K���A���r�G���g
    float4 ambient = surfaceData.albedo * 0.1f;

    //�����A���x�h�̃A���t�@�l��1.0�������Ȃ�
    if (surfaceData.albedo.w < 1.0f)
    {
        surfaceData.albedo = float4(0.0f, 0.0f, 0.0f, 0.0f);
        specular = 0.0f;
        ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);

    }
    ResultTexture[globalIndex] = float4(saturate(diffuse * surfaceData.albedo.rgb + ambient.rgb + specular), surfaceData.albedo.w);
}
    