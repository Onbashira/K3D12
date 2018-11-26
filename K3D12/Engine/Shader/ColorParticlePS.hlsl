
#if 0
#include "ColorParticleHeader.hlsl"
#endif
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};


float4 PS(PS_INPUT input) : SV_TARGET
{
    float intensity = 0.5f - length(float2(0.5f, 0.5f) - input.tex);
    intensity = clamp(intensity, 0.0f, 0.5f) * 2.0f;

    return float4(input.color.xyz, input.color.w * intensity);
}