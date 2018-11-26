
#include "ColorParticleHeader.hlsl"

GS_INPUT VS(VS_INPUT input)
{
    GS_INPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.rad = input.rad;
    output.color = input.color;
    return output;
}