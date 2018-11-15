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

    //テクスチャ向きを強制的にそろえる
    output.svPos = float4(input.texCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 1.0f, 1.0f);
    output.screenPos = input.texCoord;
    output.texCoord = input.texCoord;

}

//一次元ブラー
PSOutput XBlur(VSOutput input)
{
    PSOutput output = (PSOutput)0;

    //八近傍カーネルKからX方向にガウシアンをかける


    return output;
}


//一次元ブラー
PSOutput YBlur(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    //八近傍カーネルKからY方向にガウシアンをかける


    return output;
}

//2次元ブラー
PSOutput XYBlur(VSOutput input)
{
    PSOutput output = (PSOutput) 0;

    //八近傍カーネルKからX/Y方向にガウシアンをかける


    return output;
}