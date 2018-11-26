struct VS_INPUT
{
    float3 pos : POSITION;
    float rad : RADIUS;
    float4 color : COLOR;
};

struct GS_INPUT
{
    float4 pos : SV_POSITION;
    float rad : RADIUS;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

cbuffer cb0 : register(b0)
{
    float4x4 mWorldViewProj;
    float4x4 mInvView;
};
