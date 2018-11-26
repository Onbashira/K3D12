
struct InstanceData
{
    float4 pos[2];
    float2 colorSampleCoord;
    float2 pad;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};


StructuredBuffer<InstanceData> instanceDrawingData : register(t0);
cbuffer cb : register(b0)
{
    float4x4 wvpMat;
}

PS_INPUT VS(uint vindex : SV_VertexID, uint index : SV_InstanceID)
{
    PS_INPUT vd;
    float a = 1.0f - instanceDrawingData[index].colorSampleCoord.x;
    a *= instanceDrawingData[index].pos[vindex].w;
    vd.pos = mul(wvpMat, instanceDrawingData[index].pos[vindex]);
    vd.tex = float2(0.5f * (1.0f - vindex), 0.5f * (1.0f - vindex));
    vd.color = float4(
		1.0f - instanceDrawingData[index].colorSampleCoord.y,
		1.0f,
		1.0 - instanceDrawingData[index].colorSampleCoord.y,
		a);

    return vd;
}