#define FXAARootSignature   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
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



#define TileSize 16
//�����͍���g���Q�[���ł��łɌ��܂��Ă��邽�߂������Œ�`���Ă��\��Ȃ��͂��BCBV�]������̂��ʓ|
#define WindowWith 1920
#define WindowHeight 1080

#define TestWindowWith 1280
#define TestWindowHeight 720


#ifndef FXAA_PRESET
#define FXAA_PRESET 0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 0)
#define FXAA_EDGE_THRESHOLD      (1.0/2.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/2.0)
#define FXAA_SEARCH_STEPS        1
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       1
#define FXAA_SUBPIX_CAP          (2.5/3.0)
#define FXAA_SUBPIX_TRIM         (1.0/2.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 1)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/16.0)
#define FXAA_SEARCH_STEPS        4
#define FXAA_SEARCH_ACCELERATION 3
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 2)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        8
#define FXAA_SEARCH_ACCELERATION 2
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 3)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        16
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 4)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        24
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 5)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        32
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif

#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))

Texture2D<float4>   InputTexture    : register(t0);
SamplerState        textureSampler  : register(s0);
//rgb���͂��烋�~�i�A���~�i���X�̐���l��ԋp����
//FxaaGetLuma�i�j�͈̔͂�0.0����2.963210702�ł��B
float FxaaGetLuma(float3 rgb)
{
    return rgb.y * (0.587f / 0.299f) + rgb.x;
}

float FxaaLuminance(float4 rgba)
{
    //���̏ꍇ�A���~�i���X�l�����l�Ƃ���
    return rgba.y;
}

//���
float3 FxaaLeapFloat3(float3 a, float3 b, float amount)
{
    return (float3(-amount, -amount, -amount) * b) + ((a * float3(-amount, -amount, -amount)) + b);
}

float3 FxaaFilterReturn(float3 rgb)
{
    return rgb;
}

float4 FxaaTexLod0(float2 pos)
{
    return InputTexture.SampleLevel(textureSampler, pos.xy, 0.0f);
}

float4 FxaaTexGrad(float2 pos, float2 grad)
{
    return InputTexture.SampleGrad(textureSampler, pos.xy, grad, grad);
}

float4 FxaaTexOffset(float2 pos, int2 offset, float2 rcpFrame)
{
    return InputTexture.SampleLevel(textureSampler, pos.xy, 0.0f,offset);
}

struct VSInput
{
    float3 pos : POSITION;
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

struct FxaaTexture
{

};

[RootSignature(FXAARootSignature)]
VSOutput FxaaVSMain(VSInput input)
{
    VSOutput output;
    output.svPos = float4(input.texCoord * float2(2.0f, -2.0f) + float2(-1.0f,1.0f), 1.0f, 1.0f);
    output.screenPos = input.texCoord;
    output.texCoord = input.texCoord;
    return output;
};

[RootSignature(FXAARootSignature)]
PSOutput FxaaPSMain(VSOutput input)
{
    PSOutput output;
    float2 rcpFrame = { 1.0f / TestWindowWith, 1.0f / TestWindowHeight };
    //���[�J���P�x�`�F�b�N
    float3 rgbN = FxaaTexOffset(input.screenPos.xy, int2(0, -1), rcpFrame).xyz;
    float3 rgbW = FxaaTexOffset(input.screenPos.xy, int2(-1, 0), rcpFrame).xyz;
    float3 rgbM = FxaaTexOffset(input.screenPos.xy, int2(0, 0), rcpFrame).xyz;
    float3 rgbE = FxaaTexOffset(input.screenPos.xy, int2(1, 0), rcpFrame).xyz;
    float3 rgbS = FxaaTexOffset(input.screenPos.xy, int2(0, 1), rcpFrame).xyz;

    float lumaN = FxaaGetLuma(rgbN);
    float lumaW = FxaaGetLuma(rgbW);
    float lumaM = FxaaGetLuma(rgbM);
    float lumaE = FxaaGetLuma(rgbE);
    float lumaS = FxaaGetLuma(rgbS);

    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));

    float range = rangeMax - rangeMin;

    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
    {
        output.result = float4(FxaaFilterReturn(rgbM),1.0f);
        return output;
    }
    float3 rgbL = float3(0,0,0);
    if (FXAA_SUBPIX > 0)
    {
#if FXAA_SUBPIX_FASTER
        float3 rgbL = (rgbN + rgbW + rgbE + rgbS + rgbM) * float3(1.0f / 5.0f, 1.0 / 5.0f, 1.0f / 5.0f);
#else
        rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
#endif
    }
    //
    //�S�ߖT���[�p�X�v�Z
    //

    float lumaL = 0.0f;
    float rangeL = 0.0f;
    float blendL = 0.0f;

    #if FXAA_SUBPIX != 0
    {
        lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25f; //(N+W+E+S)/4
        rangeL = abs(lumaL - lumaM);

    }
    #endif

    #if FXAA_SUBPIX == 1
    {
        blendL = max(0.0f, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
        blendL = min(blendL, FXAA_SUBPIX_CAP);
    }
    #endif

    #if FXAA_SUBPIX == 2
    {
        blendL = rangeL / range;
    }
    #endif

    //if (FXAA_DEBUG_PASSTHROUGH)
    //{
    //}

    //
    //�����E�����T�[�`
    //
    float3 rgbNW = FxaaTexOffset(input.screenPos.xy, int2(-1, -1), rcpFrame).xyz;
    float3 rgbNE = FxaaTexOffset(input.screenPos.xy, int2(1, -1), rcpFrame).xyz;
    float3 rgbSW = FxaaTexOffset(input.screenPos.xy, int2(-1, 1), rcpFrame).xyz;
    float3 rgbSE = FxaaTexOffset(input.screenPos.xy, int2(1, 1), rcpFrame).xyz;
    
    #if ((FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0))
    
        rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
        rgbL *= float3(1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f);
    
    #endif

    float lumaNW = FxaaGetLuma(rgbNW);
    float lumaNE = FxaaGetLuma(rgbNE);
    float lumaSW = FxaaGetLuma(rgbSW);
    float lumaSE = FxaaGetLuma(rgbSE);

    float edgeVertical = abs((0.25f * lumaNW) + (-0.5f * lumaN) + (0.25f * lumaNE)) +
                         abs((0.50f * lumaW) + (-1.0f * lumaM) + (0.5f * lumaE)) +
                         abs((0.25f * lumaSW) + (-0.5f * lumaS) + (0.25f * lumaSE));

    float edgeHorizon = abs((0.25f * lumaNW) + (-0.5f * lumaW) + (0.25f * lumaSW)) +
                        abs((0.50f * lumaW) + (-1.0f * lumaM) + (0.5f * lumaE)) +
                        abs((0.25f * lumaNE) + (-0.5f * lumaE) + (0.25f * lumaSE));

    bool horizonSpan = edgeHorizon >= edgeVertical;

    float lengthSign = horizonSpan ? -rcpFrame.y : -rcpFrame.x;

    if (!(horizonSpan))
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }

    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);

    lumaN = (lumaN + lumaM) * 0.5f;
    lumaS = (lumaS + lumaM) * 0.5f;

    //
    //�O���[�h�������אڃs�N�Z���̑I���X�e�b�v
    //

    bool pairN = gradientN >= gradientS;

    if (!pairN)
    {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0f;
    }
    float2 posN;
    posN.x = input.screenPos.x + (horizonSpan ? 0.0f : lengthSign*0.5f);
    posN.y = input.screenPos.y + (horizonSpan ? lengthSign * 0.5f : 0.0f);

    //
    //�Ǐ����z�@���E�l�̌����@�Đݒ�
    //

    gradientN *= FXAA_SEARCH_THRESHOLD;
    
    //
    //���~�i���X�̃y�A�̕��ϋ������͈͊O�ɂȂ�܂ł̗������̌����r�L�΂�
    //

    float2 posP = posN;
    float2 offsetNP = horizonSpan ? float2(rcpFrame.x,0.0f) : float2(0.0f,rcpFrame.y);

    float lumaEndN = lumaN;
    float lumaEndP = lumaN;

    bool doneN = false;
    bool doneP = false;

#if(FXAA_SEARCH_ACCELERATION == 1)
    posN += offsetNP * float2(-1.0, -1.0);
    posP += offsetNP * float2(1.0, 1.0);

#endif
    
#if(FXAA_SEARCH_ACCELERATION == 2)
    posN += offsetNP * float2(-1.5,-1.5);
    posP += offsetNP * float2(1.0,1.0);
    offsetNP *= float(2.0);
#endif
    
#if(FXAA_SEARCH_ACCELERATION == 3)
    posN += offsetNP * float2(-2.0, -2.0);
    posP += offsetNP * float2(2.0, 2.0);
    offsetNP *= float(3.0);
#endif

#if(FXAA_SEARCH_ACCELERATION == 4)
    posN += offsetNP * float2(-2.5,-2.5);
    posP += offsetNP * float2(2.5,2.5);
    offsetNP *= float2(4.0,4.0);
#endif

    for (int i = 0; i < FXAA_SEARCH_STEPS;i++)
    {
        #if FXAA_SEARCH_ACCELERATION == 1
        if (!doneN)
        {
            lumaEndN = FxaaGetLuma(FxaaTexLod0(posN.xy).xyz);
        }
        if (!doneP)
        {
            lumaEndP = FxaaGetLuma(FxaaTexLod0(posP.xy).xyz);
        }
        #else
        if (!doneN)
        {
            lumaEndN = FxaaGetLuma(FxaaTexGrad(posN.xy, offsetNP).xyz);
        }
        if (!doneP)
        {
            lumaEndP = FxaaGetLuma(FxaaTexGrad(posP.xy, offsetNP).xyz);
        }
        #endif

        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);

        if (doneN &&doneP)
        {
            break;
        }
        if (!doneN)
        {
            posN -= offsetNP;
        }
        if (!doneP)
        {
            posP -= offsetNP;
        }
    }

    //
    //�s�N�Z�����X�p���̃l�K�e�B�u�����|�W�e�B�u���̂ǂ���ɂ���̂��𔻒肷��X�e�b�v
    //������dstN<dstP�Ȃ�΃l�K�e�B�u���ɂ���Ƃ�����
    //

    float dstN = horizonSpan ? input.screenPos.x - posN.x : input.screenPos.y - posN.y;
    float dstP = horizonSpan ? posP.x - input.screenPos.x : posP.y - input.screenPos.y;

    bool directionN = dstN < dstP;

    lumaEndN = directionN ? lumaEndN : lumaEndP;

    //
    //  �s�N�Z�����t�B���^�����O����Ă��邩�ǂ������m�F����X�e�b�v�G
    //

    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;

    //
    //�T�u�s�N�Z���̃I�t�Z�b�g�ƃt�B���^�X�p���̌v�Z
    //�o�ꎟ�ϊ������������
    //

    float spanLength = dstP + dstN;
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5f + (dstN * (-1.0f / spanLength))) * lengthSign;

    float3 rgbF = FxaaTexLod0(float2(input.screenPos.x + (horizonSpan ? 0.0f : subPixelOffset), input.screenPos.y + (horizonSpan ? subPixelOffset : 0.0f))).xyz;

    #if FXAA_SUBPIX == 0
    output.result = FxaaFilterReturn(rgbF);
    #else
    output.result = float4(FxaaFilterReturn(FxaaLeapFloat3(rgbL, rgbF, blendL)),1.0f);
    #endif

    return output;
};