#define ParticleRootSignature  "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                    "CBV(b0),"\
                                    "DescriptorTable(CBV(b1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "StaticSampler(s0 ,"\
                                             "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                             "addressU = TEXTURE_ADDRESS_WRAP,"\
                                             "addressV = TEXTURE_ADDRESS_WRAP,"\
                                             "addressW = TEXTURE_ADDRESS_WRAP,"\
                                             "mipLodBias = 0.0f,"\
                                             "maxAnisotropy = 16,"\
                                             "comparisonFunc  = COMPARISON_NEVER,"\
                                             "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                             "minLOD = 0.0f,"\
                                             "maxLOD = 3.402823466e+38f,"\
                                             "space  =  0,"\
                                             "visibility = SHADER_VISIBILITY_ALL"\
                                            ")"\

//define

#define MaxVertexCount 4

#include "CommonStruct.hlsl"

//�J�����̏��
cbuffer CameraInfo : register(b0)
{
    Camera camera;
};

Texture2D<float4> ParticleTex : register(t0);

SamplerState Sampler : register(s0);

struct VSInput
{
    float2 pos : POSITION; //�ʒu
    float size : SIZE; //�p�[�e�B�N���T�C�Y
    float angle : ANGLE; //�p�x
};

struct GSInput
{
    float2 pos : POSITION; //�ʒu
    float size : SIZE; //�p�[�e�B�N���T�C�Y
    float angle : ANGLE; //�p�x
};

struct GSOutput
{
    float2 pos : SV_POSITION; //�ʒu
    float2 texcoord : TEXCOORD; //�����x
};

struct PSOutput
{
    float4 color : Sv_Target;
};


GSInput VSMain(VSInput input)
{
    GSInput output = (GSInput) 0;
    output.pos = input.pos;
    output.angle = input.angle;

    return output;
};


//�W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
[maxvertexcount(MaxVertexCount)]
void CreateParticleMain
(GSInput input, inout TriangleStream<GSOutput> output)
{
    //�|���S������
    matrix vp = camera.Proj * camera.View;
    GSOutput vertex;
    float sin = 0.0f, cos = 0.0f;
    float size = input.size;
    sincos(input.angle, sin, cos);

    //��]���l�����Ȃ�
    //��꒸�_
    vertex.pos = mul(vp, float4((input.pos + (float2(-1.0f, 1.0f) * size)), 0.0f, 1.0f));
    vertex.texcoord = float2(0, 0);
    //�ǉ�
    output.Append(vertex);

    //��񒸓_
    vertex.pos = mul(vp, float4((input.pos + (float2(1.0f, 1.0f) * size)), 0.0f, 1.0f));
    vertex.texcoord = float2(1, 0);
    //�ǉ�
    output.Append(vertex);

    //��O���_
    vertex.pos = mul(vp, float4((input.pos + (float2(-1.0f, -1.0f) * size)), 0.0f, 1.0f));
    vertex.texcoord = float2(0, 1);
    //�ǉ�
    output.Append(vertex);
    
    //��l���_
    vertex.pos = mul(vp, float4((input.pos + (float2(1.0f, -1.0f) * size)), 0.0f, 1.0f));
    vertex.texcoord = float2(1, 1);
    //�ǉ�
    output.Append(vertex);

    //�X�g���b�v�j��
    output.RestartStrip();

}

PSOutput PSMain(GSOutput input)
{
    PSOutput output = (PSOutput) 0;
    output.color = ParticleTex.Sample(Sampler, input.texcoord);
    return output;
}
