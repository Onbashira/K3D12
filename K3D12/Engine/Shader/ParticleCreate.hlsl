#define ParticleRootSignature   ""
















//define

#define MaxVertexCount 4

//ここでは、いったんSOで頂点書き出し処理をするためPOSITION,NORMAL,TEXCOORDでセマンティクスを定義する
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
    float3 color : NORMAL; //初速度
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

    //第一頂点
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(0, 0);
    output.Append(vertex);

    //第二頂点
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(1, 0);
    output.Append(vertex);

    //第三頂点
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(0, 1);
    output.Append(vertex);

    //第四頂点
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(1, 1);
    output.Append(vertex);

    //ストリップ破棄
    output.RestartStrip();

}