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




//�e�X�g�Ŏg���E�C���h�E�T�C�Y
#define TestWindowWith 1280
#define TestWindowHeight 720

Texture2D<float4> InputTexture : register(t0);
SamplerState textureSampler : register(s0);

//�R���p�C���t���O

//--------------------------------------------------------------------------//

//G���P�x�Ƃ��Ĉ����R���p�C���t���O

#ifndef FXAA_GREEN_AS_LUMA
#define FXAA_GREEN_AS_LUMA 0 //1 = on,0=off
#endif

//--------------------------------------------------------------------------//
//�A���S���Y���������ŒE�o���邩�ǂ���(�I�t�ɂ���Ƃ��ڂ���Ƃ����摜�ɂȂ�j
#ifndef FXAA_EARLY_EXIT 
#define FXAA_EARLY_EXIT  0  //1 = on,0=off
#endif

//--------------------------------------------------------------------------//
//����API��Gathre���g�����ǂ��� (HLSL5���f�t�H���g�Ŏg���̂Ŏg�p����
//�t�H�[�}�b�g���Ƃɕ��������Ƃ��Ƃ�

#ifndef FXAA_GATHER4_ALPHA
#define FXAA_GATHER4_ALPHA 0
#endif

//--------------------------------------------------------------------------//
//RCPFrameQuality�ň���N�̒l�����肷��R���p�C���t���O

#ifndef FXAA_SHARPNESS_VALUE
#define FXAA_SHARPNESS_VALUE 1.0f
#endif

//--------------------------------------------------------------------------//

#ifndef FXAA_DISCARD
    //
    // PC��OpenGL�ɂ̂ݗL��
    // ���� FXAA_GREEN_AS_LUMA ���L���ȂƂ��͍쓮���Ȃ�.
    //
    // 1 = AA��K�v�Ƃ��Ȃ��s�N�Z���Ŕj�����g�p����B
    // 0 = AA��K�v�Ƃ��Ȃ��s�N�Z���ł́A�ύX����Ă��Ȃ��F��Ԃ��B
    //
#define FXAA_DISCARD 0
#endif

//����g���V�F�[�_�[�̎��
#ifndef FXAA_HLSL_5
#define FXAA_HLSL_5 1
#endif
//

/*--------------------------------------------------------------------------*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*---                      FXAA QUALITY - PRESETS                        ---*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*---                                                                    ---*/
/*--------------------------------------------------------------------------*/

#ifndef FXAA_QUALITY__PRESET
    //
    // AA�̕i���v���Z�b�g
    // OPTIONS
    // -----------------------------------------------------------------------
    // 10 to 15 - ���ʂ̕i���@10���ő���15���ō��i��
    // 20 to 29 - ���i���@20���ő���29���ō��i��
    // 39       - ���������R�X�g�B�L���C
    //
#define FXAA_QUALITY__PRESET 29
#endif
/*---------------------------------------------------------------------------*/
/*============================================================================
                     FXAA QUALITY - MEDIUM DITHER PRESETS
============================================================================*/
#if (FXAA_QUALITY__PRESET == 10)
#define FXAA_QUALITY__PS 3
#define FXAA_QUALITY__P0 1.5
#define FXAA_QUALITY__P1 3.0
#define FXAA_QUALITY__P2 12.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 11)
#define FXAA_QUALITY__PS 4
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 3.0
#define FXAA_QUALITY__P3 12.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 12)
#define FXAA_QUALITY__PS 5
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 4.0
#define FXAA_QUALITY__P4 12.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 13)
#define FXAA_QUALITY__PS 6
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 4.0
#define FXAA_QUALITY__P5 12.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 14)
#define FXAA_QUALITY__PS 7
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 4.0
#define FXAA_QUALITY__P6 12.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 15)
#define FXAA_QUALITY__PS 8
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 4.0
#define FXAA_QUALITY__P7 12.0
#endif

/*============================================================================
                     FXAA QUALITY - LOW DITHER PRESETS
============================================================================*/
#if (FXAA_QUALITY__PRESET == 20)
#define FXAA_QUALITY__PS 3
#define FXAA_QUALITY__P0 1.5
#define FXAA_QUALITY__P1 2.0
#define FXAA_QUALITY__P2 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 21)
#define FXAA_QUALITY__PS 4
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 22)
#define FXAA_QUALITY__PS 5
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 23)
#define FXAA_QUALITY__PS 6
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 24)
#define FXAA_QUALITY__PS 7
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 3.0
#define FXAA_QUALITY__P6 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 25)
#define FXAA_QUALITY__PS 8
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 4.0
#define FXAA_QUALITY__P7 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 26)
#define FXAA_QUALITY__PS 9
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 2.0
#define FXAA_QUALITY__P7 4.0
#define FXAA_QUALITY__P8 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 27)
#define FXAA_QUALITY__PS 10
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 2.0
#define FXAA_QUALITY__P7 2.0
#define FXAA_QUALITY__P8 4.0
#define FXAA_QUALITY__P9 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 28)
#define FXAA_QUALITY__PS 11
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 2.0
#define FXAA_QUALITY__P7 2.0
#define FXAA_QUALITY__P8 2.0
#define FXAA_QUALITY__P9 4.0
#define FXAA_QUALITY__P10 8.0
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PRESET == 29)
#define FXAA_QUALITY__PS 12
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.5
#define FXAA_QUALITY__P2 2.0
#define FXAA_QUALITY__P3 2.0
#define FXAA_QUALITY__P4 2.0
#define FXAA_QUALITY__P5 2.0
#define FXAA_QUALITY__P6 2.0
#define FXAA_QUALITY__P7 2.0
#define FXAA_QUALITY__P8 2.0
#define FXAA_QUALITY__P9 2.0
#define FXAA_QUALITY__P10 4.0
#define FXAA_QUALITY__P11 8.0
#endif

/*============================================================================
                     FXAA QUALITY - EXTREME QUALITY
============================================================================*/
//�������d������������@�Q�[���ɂ͌����Ă��Ȃ��̂���
#if (FXAA_QUALITY__PRESET == 39)
#define FXAA_QUALITY__PS 12
#define FXAA_QUALITY__P0 1.0
#define FXAA_QUALITY__P1 1.0
#define FXAA_QUALITY__P2 1.0
#define FXAA_QUALITY__P3 1.0
#define FXAA_QUALITY__P4 1.0
#define FXAA_QUALITY__P5 1.5
#define FXAA_QUALITY__P6 2.0
#define FXAA_QUALITY__P7 2.0
#define FXAA_QUALITY__P8 2.0
#define FXAA_QUALITY__P9 2.0
#define FXAA_QUALITY__P10 4.0
#define FXAA_QUALITY__P11 8.0
#endif

//���Ƃő����̃v���b�g�t�H�[���ɑΉ��ł���悤�ɂ��Ă���


//--------------------------------------------------------------------------//

#if FXAA_HLSL_5 == 1

#define FxaaInt2 int2
#define FxaaTexTop(p)            InputTexture.SampleLevel(textureSampler, p, 0.0)
#define FxaaTexOff(p, o, r)      InputTexture.SampleLevel(textureSampler, p, 0.0, o)
#define FxaaTexAlpha4(p)         InputTexture.GatherAlpha(textureSampler, p)
#define FxaaTexOffAlpha4(p, o)   InputTexture.GatherAlpha(textureSampler, p, o)
#define FxaaTexGreen4(p)         InputTexture.GatherGreen(textureSampler, p)
#define FxaaTexOffGreen4(p, o)   InputTexture.GatherGreen(textureSampler, p, o)
#endif
//--------------------------------------------------------------------------//

//rgb���͂��烋�~�i���X�̐���l��ԋp����
//FxaaGetLuma�i�j�͈̔͂�0.0����2.963210702�ł��B
#if (FXAA_GREEN_AS_LUMA == 1)
float FxaaLuma(float4 rgba) {
    return rgba.y;

}
#else
float FxaaLuma(float4 rgba)
{
    return 0.299f * rgba.x + rgba.y * 0.587f + rgba.z * 0.114f;
}
#endif  

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
    return InputTexture.SampleLevel(textureSampler, pos.xy, 0.0f, offset);
}

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


//VertexShadermain
[RootSignature(FXAARootSignature)]
VSOutput FxaaVSMain(VSInput input)
{
    VSOutput output;
    output.svPos = float4(input.texCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 1.0f, 1.0f);
    output.screenPos = input.texCoord;
    output.texCoord = input.texCoord;
    return output;
};

//PixelShadermain
[RootSignature(FXAARootSignature)]
PSOutput FxaaPSMain(VSOutput input)
{
    PSOutput output;
    //RCP�t���[���̐ݒ�i���CBV�Ή��ɂ��邩�ǂ��������߂�
    float2 rcpFrame = { 1.0f / TestWindowWith, 1.0f / TestWindowHeight };
    //fxaaConsoleRcpFrameQuality
    float4 fxaaQualityRcpFrame =
    {
        FXAA_SHARPNESS_VALUE / TestWindowWith, FXAA_SHARPNESS_VALUE / TestWindowHeight,
       -FXAA_SHARPNESS_VALUE / TestWindowWith, -FXAA_SHARPNESS_VALUE / TestWindowHeight
    };

    float4 rcpOptFrame = { -2.0f / TestWindowWith, -2.0f / TestWindowHeight, 2.0f / TestWindowWith, 2.0f / TestWindowHeight };
    float4 rcpOptFrame2 = { -8.0f / TestWindowWith, -8.0f / TestWindowHeight, 4.0f / TestWindowWith, 4.0f / TestWindowHeight };
  
    //��Œ萔�Ō��߂��悤�ɕύX

    float fxaaSubPixelQuality = 0.125f;
    float fxaaQualityEdgeThreshold = 0.125f;
    float fxaaQualityEdgeThresholdMin = 0.0625f;
    float fxaaEdgeSharpness = 4.0f;
    float fxaaEdgeThreshold = 0.125f;
    float fxaaEdgeThresholdMin = 0.05f;
    //
    
    float2 posM = input.screenPos.xy;

    //���[�J���P�x�`�F�b�N
    
#if (FXAA_GATHER4_ALPHA == 1)
    #if (FXAA_DISCARD == 0)
        float4 rgbyM = FxaaTexTop(posM);
        #if (FXAA_GREEN_AS_LUMA == 0)

            #define lumaM FxaaLuma(rgbyM)

        #else

            #define lumaM rgbyM.y

        #endif

    #endif
    #if (FXAA_GREEN_AS_LUMA == 1)
        float4 luma4A = FxaaTexGreen4(posM);
        float4 luma4B = FxaaTexOffGreen4(posM, int2(-1, -1));
       
    #else
        float4 luma4A = FxaaTexAlpha4(posM);
        float4 luma4B = FxaaTexOffAlpha4( posM, int2(-1, -1));
    #endif
    #if (FXAA_DISCARD == 1)
     #define lumaM luma4A.w
    #endif
    float lumaE = luma4A.z;
    float lumaS = luma4A.x;
    float lumaSE = luma4A.y;
    float lumaNW = luma4B.w;
    float lumaN = luma4B.z;
    float lumaW = luma4B.x;
#else
    float4 rgbyM = FxaaTexTop(posM);
    #if (FXAA_GREEN_AS_LUMA == 0)
        #define lumaM FxaaLuma(rgbyM)
    #else
        #define lumaM rgbyM.y
    #endif
    float lumaS = FxaaLuma(FxaaTexOff(posM, int2( 0, 1), fxaaQualityRcpFrame.xy));
    float lumaE = FxaaLuma(FxaaTexOff(posM, int2( 1, 0), fxaaQualityRcpFrame.xy));
    float lumaN = FxaaLuma(FxaaTexOff(posM, int2( 0,-1), fxaaQualityRcpFrame.xy));
    float lumaW = FxaaLuma(FxaaTexOff(posM, int2(-1, 0), fxaaQualityRcpFrame.xy));
#endif

   

    //
    //�S�ߖT���[�p�X�v�Z
    //

    float maxSM = max(lumaS, lumaM);
    float minSM = min(lumaS, lumaM);
    float maxESM = max(lumaE, maxSM);
    float minESM = min(lumaE, minSM);
    float maxWN = max(lumaN, lumaW);
    float minWN = min(lumaN, lumaW);
    float rangeMax = max(maxWN, maxESM);
    float rangeMin = min(minWN, minESM);
    float rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;
    float range = rangeMax - rangeMin;
    float rangeMaxClamped = max(rangeMaxScaled, fxaaQualityEdgeThresholdMin);
    bool earlyExit = range < rangeMaxClamped;

    if (earlyExit)
    {
#if (FXAA_DISCARD == 1)
            discard;
#else
        output.result = float4(FxaaFilterReturn(rgbyM.xyz), 1.0f);
        return output;
#endif
    }
  

    //
    //�����E�����T�[�`
    //

#if(FXAA_GATHER4_ALPHA == 0)
       float lumaNW = FxaaLuma(FxaaTexOff(posM, int2(-1, -1), fxaaQualityRcpFrame.xy));
       float lumaSE = FxaaLuma(FxaaTexOff(posM, int2( 1, 1), fxaaQualityRcpFrame.xy));
       float lumaNE = FxaaLuma(FxaaTexOff(posM, int2( 1,-1), fxaaQualityRcpFrame.xy));
       float lumaSW = FxaaLuma(FxaaTexOff(posM, int2(-1, 1), fxaaQualityRcpFrame.xy));
#else
    float lumaNE = FxaaLuma(FxaaTexOff(posM, int2( 1,-1), fxaaQualityRcpFrame.xy));
    float lumaSW = FxaaLuma(FxaaTexOff(posM, int2(-1, 1), fxaaQualityRcpFrame.xy));
#endif

    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float subpixRcpRange = 1.0f / range;
    float subpixNSWE = lumaW + lumaE;
    float edgeHorizon1 = (-2.0f * lumaM) + lumaNS;
    float edgeVertical1 = (-2.0f * lumaM) + lumaWE;

    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorizon2 = (-2.0f * lumaE) + lumaNESE;
    float edgeVertical2 = (-2.0f * lumaN) + lumaNWNE;

    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz4 = (abs(edgeHorizon1) * 2.0f) + abs(edgeHorizon2);
    float edgeVert4 = (abs(edgeVertical1) * 2.0f) + abs(edgeVertical2);
    float edgeHorz3 = (-2.0f * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0f * lumaS) + lumaSWSE;
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;

    float subpixNWSWNESE = lumaNWSW + lumaNESE;
    float lengthSign = fxaaQualityRcpFrame.x;
    float horzSpan = edgeHorz >= edgeVert;
    float subpixA = subpixNSWE * 2.0f + subpixNWSWNESE;

    if (!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }
    if (horzSpan)
    {
        lengthSign = fxaaQualityRcpFrame.y;
    }
    float subpixB = (subpixA * (1.0f / 12.0f)) - lumaM;

     //
    //�O���[�h�������אڃs�N�Z���̑I���X�e�b�v
    //

    float gradientN = lumaN - lumaM;
    float gradientS = lumaS - lumaM;
    float lumaNN = lumaN + lumaM;
    float lumaSS = lumaS + lumaM;
    bool pairN = abs(gradientN) >= abs(gradientS);
    float gradient = max(abs(gradientN), abs(gradientS));
    if (pairN)
        lengthSign = -lengthSign;
    float subpixC = saturate(abs(subpixB) * subpixRcpRange);

    float2 posB;
    posB.x = posM.x;
    posB.y = posM.y;
    float2 offNP;
    offNP.x = (!horzSpan) ? 0.0f : fxaaQualityRcpFrame.x;
    offNP.y = (horzSpan) ? 0.0f: fxaaQualityRcpFrame.y;
    if (!horzSpan)
        posB.x += lengthSign * 0.5f;
    if (horzSpan)
        posB.y += lengthSign * 0.5f;

    float2 posN;
    posN.x = posB.x - offNP.x * FXAA_QUALITY__P0;
    posN.y = posB.y - offNP.y * FXAA_QUALITY__P0;
    float2 posP;
    posP.x = posB.x + offNP.x * FXAA_QUALITY__P0;
    posP.y = posB.y + offNP.y * FXAA_QUALITY__P0;
    float subpixD = ((-2.0f) * subpixC) + 3.0f;
    float lumaEndN = FxaaLuma(FxaaTexTop(posN));
    float subpixE = subpixC * subpixC;
    float lumaEndP = FxaaLuma(FxaaTexTop(posP));

    //
    //���~�i���X�̃y�A�̕��ϋ������͈͊O���T���v�����O���ɂȂ�܂ł̗������̌����r�L�΂�
    //
    
    if (!pairN)
    {
        lumaNN = lumaSS;
    }
    float gradientScaled = gradient * 1.0f / 4.0f;
    float lumaMM = lumaM - lumaNN * 0.5f;
    float subpixF = subpixD * subpixE;
    bool lumaMLTZero = lumaMM < 0.0f;

    lumaEndN -= lumaNN * 0.5f;
    lumaEndP -= lumaNN * 0.5f;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if (!doneN)
        posN.x -= offNP.x * FXAA_QUALITY__P1;
    if (!doneN)
        posN.y -= offNP.y * FXAA_QUALITY__P1;
    bool doneNP = (!doneN) || (!doneP);
    if (!doneP)
        posP.x += offNP.x * FXAA_QUALITY__P1;
    if (!doneP)
        posP.y += offNP.y * FXAA_QUALITY__P1;

    //�T�u�s�N�Z���T���v�����O
    if (doneNP)
    {
        if (!doneN)
            lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
        if (!doneP)
            lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
        if (!doneN)
            lumaEndN = lumaEndN - lumaNN * 0.5f;
        if (!doneP)
            lumaEndP = lumaEndP - lumaNN * 0.5f;
        doneN = abs(lumaEndN) >= gradientScaled;
        doneP = abs(lumaEndP) >= gradientScaled;
        if (!doneN)
            posN.x -= offNP.x * FXAA_QUALITY__P2;
        if (!doneN)
            posN.y -= offNP.y * FXAA_QUALITY__P2;
        doneNP = (!doneN) || (!doneP);
        if (!doneP)
            posP.x += offNP.x * FXAA_QUALITY__P2;
        if (!doneP)
            posP.y += offNP.y * FXAA_QUALITY__P2;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 3)
        if (doneNP)
        {
            if (!doneN)
                lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
            if (!doneP)
                lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
            if (!doneN)
                lumaEndN = lumaEndN - lumaNN * 0.5f;
            if (!doneP)
                lumaEndP = lumaEndP - lumaNN * 0.5f;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if (!doneN)
                posN.x -= offNP.x * FXAA_QUALITY__P3;
            if (!doneN)
                posN.y -= offNP.y * FXAA_QUALITY__P3;
            doneNP = (!doneN) || (!doneP);
            if (!doneP)
                posP.x += offNP.x * FXAA_QUALITY__P3;
            if (!doneP)
                posP.y += offNP.y * FXAA_QUALITY__P3;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 4)
            if (doneNP)
            {
                if (!doneN)
                    lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                if (!doneP)
                    lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                if (!doneN)
                    lumaEndN = lumaEndN - lumaNN * 0.5f;
                if (!doneP)
                    lumaEndP = lumaEndP - lumaNN * 0.5f;
                doneN = abs(lumaEndN) >= gradientScaled;
                doneP = abs(lumaEndP) >= gradientScaled;
                if (!doneN)
                    posN.x -= offNP.x * FXAA_QUALITY__P4;
                if (!doneN)
                    posN.y -= offNP.y * FXAA_QUALITY__P4;
                doneNP = (!doneN) || (!doneP);
                if (!doneP)
                    posP.x += offNP.x * FXAA_QUALITY__P4;
                if (!doneP)
                    posP.y += offNP.y * FXAA_QUALITY__P4;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 5)
                if(doneNP) {
                    if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                    if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                    doneN = abs(lumaEndN) >= gradientScaled;
                    doneP = abs(lumaEndP) >= gradientScaled;
                    if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P5;
                    if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P5;
                    doneNP = (!doneN) || (!doneP);
                    if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P5;
                    if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P5;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 6)
                    if(doneNP) {
                        if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                        if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                        doneN = abs(lumaEndN) >= gradientScaled;
                        doneP = abs(lumaEndP) >= gradientScaled;
                        if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P6;
                        if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P6;
                        doneNP = (!doneN) || (!doneP);
                        if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P6;
                        if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P6;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 7)
                        if(doneNP) {
                            if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                            if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                            doneN = abs(lumaEndN) >= gradientScaled;
                            doneP = abs(lumaEndP) >= gradientScaled;
                            if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P7;
                            if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P7;
                            doneNP = (!doneN) || (!doneP);
                            if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P7;
                            if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P7;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 8)
                            if(doneNP) {
                                if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                                if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                                doneN = abs(lumaEndN) >= gradientScaled;
                                doneP = abs(lumaEndP) >= gradientScaled;
                                if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P8;
                                if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P8;
                                doneNP = (!doneN) || (!doneP);
                                if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P8;
                                if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P8;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 9)
                                if(doneNP) {
                                    if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                                    if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                                    doneN = abs(lumaEndN) >= gradientScaled;
                                    doneP = abs(lumaEndP) >= gradientScaled;
                                    if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P9;
                                    if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P9;
                                    doneNP = (!doneN) || (!doneP);
                                    if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P9;
                                    if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P9;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 10)
                                    if(doneNP) {
                                        if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                                        if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                                        doneN = abs(lumaEndN) >= gradientScaled;
                                        doneP = abs(lumaEndP) >= gradientScaled;
                                        if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P10;
                                        if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P10;
                                        doneNP = (!doneN) || (!doneP);
                                        if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P10;
                                        if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P10;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 11)
                                        if(doneNP) {
                                            if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                                            if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                                            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                                            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                                            doneN = abs(lumaEndN) >= gradientScaled;
                                            doneP = abs(lumaEndP) >= gradientScaled;
                                            if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P11;
                                            if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P11;
                                            doneNP = (!doneN) || (!doneP);
                                            if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P11;
                                            if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P11;
/*--------------------------------------------------------------------------*/
#if (FXAA_QUALITY__PS > 12)
                                            if(doneNP) {
                                                if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(posN.xy));
                                                if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(posP.xy));
                                                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5f;
                                                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5f;
                                                doneN = abs(lumaEndN) >= gradientScaled;
                                                doneP = abs(lumaEndP) >= gradientScaled;
                                                if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P12;
                                                if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P12;
                                                doneNP = (!doneN) || (!doneP);
                                                if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P12;
                                                if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P12;
/*--------------------------------------------------------------------------*/
                                          �@}
#endif
/*--------------------------------------------------------------------------*/
                                        }
#endif
/*--------------------------------------------------------------------------*/
                                    }
#endif
/*--------------------------------------------------------------------------*/
                                }
#endif
/*--------------------------------------------------------------------------*/
                            }
#endif
/*--------------------------------------------------------------------------*/
                        }
#endif
/*--------------------------------------------------------------------------*/
                    }
#endif
/*--------------------------------------------------------------------------*/
                }
#endif
/*--------------------------------------------------------------------------*/
            }
#endif
/*--------------------------------------------------------------------------*/
        }
#endif
/*--------------------------------------------------------------------------*/
    }
   
    //
    //�s�N�Z�����X�p���̃l�K�e�B�u�����|�W�e�B�u���̂ǂ���ɂ���̂��𔻒肷��X�e�b�v
    //������dstN<dstP�Ȃ�΃l�K�e�B�u���ɂ���Ƃ�����
    //

    float dstN = posM.x - posN.x;
    float dstP = posP.x - posM.x;
    if (!horzSpan)
        dstN = posM.y - posN.y;
    if (!horzSpan)
        dstP = posP.y - posM.y;

    bool goodSpanN = (lumaEndN < 0.0f) != lumaMLTZero;
    float spanLength = (dstP + dstN);
    bool goodSpanP = (lumaEndP < 0.0f) != lumaMLTZero;
    float spanLengthRcp = 1.0f / spanLength;

    //
    //�s�N�Z�����t�B���^�����O����Ă��邩�ǂ������m�F����X�e�b�v�G
    //

    bool directionN = dstN < dstP;
    float dst = min(dstN, dstP);
    bool goodSpan = directionN ? goodSpanN : goodSpanP;
    float subpixG = subpixF * subpixF;
    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5f;
    float subpixH = subpixG * fxaaSubPixelQuality;

    //
    //�T�u�s�N�Z���̃I�t�Z�b�g�ƃt�B���^�X�p���̌v�Z
    //�o�ꎟ�ϊ������������
    //
    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0f;
    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
    if (!horzSpan)
        posM.x += pixelOffsetSubpix * lengthSign;
    if (horzSpan)
        posM.y += pixelOffsetSubpix * lengthSign;
#if (FXAA_DISCARD == 1)
        return FxaaTexTop(tex, posM);
#else
    output.result = float4(FxaaTexTop(posM).xyz, lumaM);
#endif
    return output;
};