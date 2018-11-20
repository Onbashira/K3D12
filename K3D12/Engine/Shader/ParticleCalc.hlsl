#define ParticleCalcRootSignature   "RootFlags(0),"\
                                    "CBV(0),"\
                                    "DescriptorTable(SRV(t0,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u1,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u2,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u3,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\



//GPU�A�h���X�ł����̂�Root�n�o�C���h�����A�f�X�N���v�^�[�e�[�u���Ńo�C���h����p�����[�^��DESCRIPTORHEAP��GPU�A�h���X���ۂ��E�E�E�H


struct Particle
{
    float4 pos;
    float2 velocity;
    float angle;
    float lifeTime;
};

struct IndirectCommand
{
    uint2 VertexViewAddress;
    uint2 CameraGPUAddress;
    uint2 ShaderResourceAddress;
    uint4 DrawArguments;
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
    uint padding01;
    uint padding02;
};

//�ǂݍ��ݐ�p�Œ蒷�z��
StructuredBuffer<Particle> ParticleCreationData : register(t0);

//�p�[�e�B�N���������݃o�b�t�@�Œ蒷�z��
RWStructuredBuffer<Particle> ParticleData : register(u0);

//�R�}���h�o�b�t�@
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u1);

//�󂫃C���f�b�N�X�o�b�t�@(+Append/Consume)
ConsumeStructuredBuffer<uint> reserveSlotsConsume : register(u2);
AppendStructuredBuffer<uint> reserveSlotsAppend : register(u3);

[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_1024, 1, 1)]
void ResetReserveBuffer(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupID.x * THREAD_X) + groupThreadID.x;
    
    //�󂫂�S���C���f�b�N�X�Ŗ��߂�
    if (index < CommandCount)
    {
        reserveSlotsAppend.Append(index);
    }
}


//�p�[�e�B�N������
[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_1024, 1, 1)]
void SpawnParticle(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupID.x * THREAD_X) + groupThreadID.x;
    [branch]
    if (index < SpawnCount)
    {
        uint newIndex = reserveSlotsConsume.Consume();
        ParticleData[newIndex].pos = ParticleCreationData[index].pos;
        ParticleData[newIndex].velocity = ParticleCreationData[index].velocity;
        ParticleData[newIndex].angle = ParticleCreationData[index].angle;
        ParticleData[newIndex].lifeTime = ParticleCreationData[index].lifeTime;
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
[numthreads(THREAD_1024, 1, 1)]
void UpdateParticles(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupID.x * THREAD_X) + groupThreadID.x;
    [branch] //���S���Ă��Ȃ��Ȃ�
    if (ParticleData[index].pos.w > 0.0f)
    {
    
        [branch] //�����c���������Ԃ�0.0���傫�����
        if (ParticleData[index].lifeTime > 0.0f)
        {
            UpdateParticle(ParticleData[index], 0.016f);
        }
        else
        {
            ParticleData[index].pos.w = -1.0f; //kill Particle
            reserveSlotsAppend.Append(index); //�󂫃C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�l���A�y���h
        }
    }

}
