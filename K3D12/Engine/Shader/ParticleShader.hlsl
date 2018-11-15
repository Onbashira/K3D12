#define ParticleRootSignature   ""
















//define

#define MaxVertexCount 4

#include "CommonStruct.hlsl"

//�J�����̏��
cbuffer CameraInfo : register(b0)
{
    Camera camera;
};

Texture2D<float4> ParticleTex : register(t0);

SamplerState Sampler : register(s0);

struct VSOutput
{
    float2 pos : POSITION; //�ʒu
    float2 texcoord : TEXCOORD; //�����x
    float2 velocity : VELOCITY; //�����x
    float angle : ANGLE; //�p�x
    float liveTime : LIVETIME; //��������
};

typedef VSOutput VSInput;

struct GSOutput
{
    float2 pos : SV_POSITION; //�ʒu
    float2 texcoord : TEXCOORD; //�����x
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


//�W�I���g���V�F�[�_�[�ŏo�͂���ő咸�_��
[maxvertexcount(MaxVertexCount)]
void CreateParticleMain
(GSInput input, inout TriangleStream<GSOutput> output)
{
    //�|���S������

    GSOutput vertex;

    //��꒸�_
    vertex.pos;
    vertex.texcoord = float2(0, 0);
    //�ǉ�
    output.Append(vertex);

    //��񒸓_
    vertex.pos;
    vertex.texcoord = float2(1, 0);
    //�ǉ�
    output.Append(vertex);

    //��O���_
    vertex.pos;
    vertex.texcoord = float2(0, 1);
    //�ǉ�
    output.Append(vertex);
    
    //��l���_
    vertex.pos;
    vertex.texcoord = float2(1, 1);
    //�ǉ�
    output.Append(vertex);

    //�X�g���b�v�j��
    output.RestartStrip();

}

PSOutput PSMain(GSOutput input)
{
    PSOutput output = (PSOutput)0;
    output.color = ParticleTex.Sample(Sampler, input.texcoord);
    return output;
}
