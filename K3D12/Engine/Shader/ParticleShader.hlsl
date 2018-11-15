#define ParticleRootSignature   ""
















//define

#define MaxVertexCount 4

#include "CommonStruct.hlsl"

//カメラの情報
cbuffer CameraInfo : register(b0)
{
    Camera camera;
};

Texture2D<float4> ParticleTex : register(t0);

SamplerState Sampler : register(s0);

struct VSOutput
{
    float2 pos : POSITION; //位置
    float2 texcoord : TEXCOORD; //初速度
    float2 velocity : VELOCITY; //初速度
    float angle : ANGLE; //角度
    float liveTime : LIVETIME; //生存時間
};

typedef VSOutput VSInput;

struct GSOutput
{
    float2 pos : SV_POSITION; //位置
    float2 texcoord : TEXCOORD; //初速度
};

typedef GSOutput GSInput;

struct PSOutput
{
    float4 color : Sv_Target;
};


VSOutput VSMain(VSInput input)
{
    return input;
};


//ジオメトリシェーダーで出力する最大頂点数
[maxvertexcount(MaxVertexCount)]
void CreateParticleMain
(GSInput input, inout TriangleStream<GSOutput> output)
{
    //板ポリゴン生成

    GSOutput vertex;

    //第一頂点
    vertex.pos;
    vertex.texcoord = float2(0, 0);
    //追加
    output.Append(vertex);

    //第二頂点
    vertex.pos;
    vertex.texcoord = float2(1, 0);
    //追加
    output.Append(vertex);

    //第三頂点
    vertex.pos;
    vertex.texcoord = float2(0, 1);
    //追加
    output.Append(vertex);
    
    //第四頂点
    vertex.pos;
    vertex.texcoord = float2(1, 1);
    //追加
    output.Append(vertex);

    //ストリップ破棄
    output.RestartStrip();

}

PSOutput PSMain(GSOutput input)
{
    PSOutput output = (PSOutput)0;
    output.color = ParticleTex.Sample(Sampler, input.texcoord);
    return output;
}
