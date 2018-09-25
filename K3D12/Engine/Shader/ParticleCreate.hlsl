#define ParticleRootSignature   ""
















//define

#define MaxVertexCount 4

//�����ł́A��������SO�Œ��_�����o�����������邽��POSITION,NORMAL,TEXCOORD�ŃZ�}���e�B�N�X���`����
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
    float3 color : NORMAL; //�����x
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

    //��꒸�_
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(0, 0);
    output.Append(vertex);

    //��񒸓_
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(1, 0);
    output.Append(vertex);

    //��O���_
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(0, 1);
    output.Append(vertex);

    //��l���_
    vertex.pos;
    vertex.color;
    vertex.texcoord = float2(1, 1);
    output.Append(vertex);

    //�X�g���b�v�j��
    output.RestartStrip();

}