#define GaussianFilterRootSignature   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                             "DescriptorTable(SRV(t0,numDescriptors = 1, space = 0)," \
                                             "visibility = SHADER_VISIBILITY_ALL),"  \
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
                                             "),"\


Texture2D<float4> inputTexture : register(s0);
SamplerState samp : register(s0);


struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float2 screenPos : CONVERTED_POSITOIN;
    float2 texCoord : TEXCOORD;
};

struct PSOutput
{
    float4 result : SV_Target0;
};


float GetLuma(float4 rgba)
{
    return 0.299f * rgba.x + rgba.y * 0.587f + rgba.z * 0.114f;
}

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput)0;

    //�e�N�X�`�������������I�ɂ��낦��
    output.svPos = float4(input.texCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 1.0f, 1.0f);
    output.screenPos = input.texCoord;
    output.texCoord = input.texCoord;

}

//�ꎟ���u���[
PSOutput XBlur(VSOutput input)
{
    PSOutput output = (PSOutput)0;

    //���ߖT�J�[�l��K����X�����ɃK�E�V�A����������


    return output;
}


//�ꎟ���u���[
PSOutput YBlur(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    //���ߖT�J�[�l��K����Y�����ɃK�E�V�A����������


    return output;
}

//2�����u���[
PSOutput XYBlur(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    //���ߖT�J�[�l��K����X/Y�����ɃK�E�V�A����������


    return output;
}