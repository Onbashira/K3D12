
#include "ColorParticleHeader.hlsl"

cbuffer cbImmutable
{
    static float3 s_positions[4] =
    {
        float3(-1, 1, 0),
		float3(1, 1, 0),
		float3(-1, -1, 0),
		float3(1, -1, 0),
    };

    static float2 s_texcoords[4] =
    {
        float2(0, 0),
		float2(1, 0),
		float2(0, 1),
		float2(1, 1),
    };
};

[maxvertexcount(4)]
void GS(
	point GS_INPUT input[1],
	inout TriangleStream<PS_INPUT> SpriteStream
)
{
    PS_INPUT output;

    for (uint i = 0; i < 4; i++)
    {
        float3 pos = s_positions[i] * input[0].rad;
        pos = mul((float3x3) mInvView, pos);
        pos += input[0].pos.xyz;
        output.pos = mul(mWorldViewProj, float4(pos, 1.0f));
        output.color = input[0].color;
        output.tex = s_texcoords[i];
        SpriteStream.Append(output);
    }

    SpriteStream.RestartStrip();
}