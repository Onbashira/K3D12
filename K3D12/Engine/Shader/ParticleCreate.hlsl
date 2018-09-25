
















//define

#define MaxVertexCount 4

struct VSOutput
{
    float3 pos : POSITION; //�ʒu
    float3 velocity : NORMAL; //�����x
    float time : TEXCOORD; //��������
};

typedef VSOutput VSInput;


struct GSOutput
{
    float4 pos : SV_POSITION; //�ʒu
    float3 velocity : NORMAL; //�����x
    float2 texcoord : TEXCOORD; //��������
};

typedef GSOutput GSInput;

//���̂܂ܖ߂�
VSOutput VSMain(VSInput input)
{
    return input;
};


//�W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
[maxvertexcount(MaxVertexCount)]
void CreateParticleMain
(GSInput input, inout TriangleStream<GSOutput> output)
{
    //�|���S������

    GSOutput vertex;

    for (int i = 0; i < MaxVertexCount;++i)
    {
        vertex.pos;
        vertex.velocity;
        vertex.time;
        output.Append(vertex);
    }

        output.Append(vertex);




    //�X�g���b�v�j��
    output.RestartStrip();

}