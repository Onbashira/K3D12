
















//define

#define MaxVertexCount 4

struct VSOutput
{
    float3 pos : POSITION; //位置
    float3 velocity : NORMAL; //初速度
    float time : TEXCOORD; //生存時間
};

typedef VSOutput VSInput;


struct GSOutput
{
    float4 pos : SV_POSITION; //位置
    float3 velocity : NORMAL; //初速度
    float2 texcoord : TEXCOORD; //生存時間
};

typedef GSOutput GSInput;

//そのまま戻す
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

    for (int i = 0; i < MaxVertexCount;++i)
    {
        vertex.pos;
        vertex.velocity;
        vertex.time;
        output.Append(vertex);
    }

        output.Append(vertex);




    //ストリップ破棄
    output.RestartStrip();

}