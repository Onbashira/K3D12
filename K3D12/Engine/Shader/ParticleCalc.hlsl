#define ParticleCalcRootSignature   "RootFlags(0),"\
                                    "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\


struct Particle
{
    float4 pos;
    float2 velocity;
    float angle;
    float lifeTime;
};

struct IndirectCommand
{
    uint2 cameraAddress;
    uint2 textureAddress;
    uint4 drawArguments;
};

#define THREAD_X 128
#define THREAD_Y 1

#define THREAD_1024 1024
#define THREAD_256  256
#define THREAD_128  128
#define THREAD_64   64

cbuffer SceneRootConstant : register(b0)
{
    uint SpawnCount; //�ő吶����
    uint CommandCount; //�ő�R�}���h������
};

//�ǂݍ��ݐ�p�Œ蒷�z��
StructuredBuffer<Particle> ParticleCreationData : register(t0);

//�p�[�e�B�N���������݃o�b�t�@�Œ蒷�z��
RWStructuredBuffer<Particle> ParticleData : register(u0);

//�R�}���h�o�b�t�@
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u1);

//�󂫃C���f�b�N�X�o�b�t�@(+Append/Consume)
ConsumeStructuredBuffer<uint> reserveSlotsConsume : register(t1);
AppendStructuredBuffer<uint> reserveSlotsAppend : register(u1);

[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_X, 1, 1)]
void ResetReserveBuffer(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    reserveSlotsAppend.Append(dispatchThreadID.x);
}


//�p�[�e�B�N������
[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_X, THREAD_Y, 1)]
void SpawnParticle(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    [branch]
    if (dispatchThreadID.x < SpawnCount)
    {
        uint newIndex = reserveSlotsConsume.Consume();
        ParticleData[newIndex].pos = ParticleCreationData[dispatchThreadID.x].pos;
        ParticleData[newIndex].velocity = ParticleCreationData[dispatchThreadID.x].velocity;
        ParticleData[newIndex].angle = ParticleCreationData[dispatchThreadID.x].angle;
        ParticleData[newIndex].lifeTime = ParticleCreationData[dispatchThreadID.x].lifeTime;
    }
}

//�p�[�e�B�N���P�̍X�V
void UpdateParticle(inout Particle particle, float lifetime)
{
    particle.lifeTime -= lifetime;
    particle.velocity *= 0.8f;
    particle.pos += particle.velocity;
    particle.angle = 0.0f;
}

//�p�[�e�B�N���X�V
[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_X, THREAD_Y, 1)]
void UpdateParticles(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupID.x * THREAD_X) + groupThreadID.x;
    [branch] //���S���Ă��Ȃ��Ȃ�
    if (ParticleData[index].pos.w > 0.0f)
    {
    
        [branch] //�����c���������Ԃ�0.0���傫�����
        if (ParticleData[index].lifeTime > 0.0f )
        {
            UpdateParticle(ParticleData[index], 0.016f);
        }
        else
        {
            ParticleData[index].pos.w = -1.0f; //kill Particle
            reserveSlotsAppend.Append(index);�@//�󂫃C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�l���A�y���h
        }
    }

}
