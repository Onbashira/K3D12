#define ParticleCalcRootSignature   "RootFlags(0),"\
                                    "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\


struct Particle
{
    float2 pos;
    float2 velocity;
    float angle;
    float lifeTime;
};

#define ADDRESS_OFFSET_POSITION 16
#define ADDRESS_OFFSET_VELOCITY 32
#define ADDRESS_OFFSET_ANGLE    40
#define ADDRESS_OFFSET_LIFETIME 48
#define THREAD_X 100
#define THREAD_Y 100

//�ǂݍ��ݐ�p
StructuredBuffer<Particle> ParticleCreationData : register(t0);
//�p�[�e�B�N���������݃o�b�t�@
RWStructuredBuffer<Particle> ParticleData : register(u0);
//�󂫃C���f�b�N�X�̎Q�ƃo�b�t�@
ConsumeStructuredBuffer<uint> ParticleReleasedConsumeBuffer : register(t1);
RWByteAddressBuffer ParticleReleasedAppendBuffer : register(u1);

[RootSignature(ParticleCalcRootSignature)]
[numthreads(THREAD_X, THREAD_Y, 1)]
void main(uint3 groupeID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    
}