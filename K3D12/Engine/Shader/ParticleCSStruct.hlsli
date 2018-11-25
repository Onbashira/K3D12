struct SpawnData
{
    float3 forward;
    float colorSamplingV;
    float4 position;
    float initialSpeedFactor;
    float reductionRate;
    float speedMag;
    float lengthMag;
};

struct InstanceData
{
    float4 position;
    float3 forward;
    float initialSpeedFactor;
    float reductionRate;
    float speedMag;
    float lengthMag;
    float spawnTime;
    float3 reserved;
    float colorSamplingV;
};

struct InstanceDrawData
{
    float4 position[2];
    float2 colorSampleCoord;
    float2 pad;
};

#ifdef PARTICLE_SPAWN
ConsumeStructuredBuffer<uint> reservedSlots	 : register(u0);
#else
AppendStructuredBuffer<uint> reservedSlotsApp : register(u0);
#endif
AppendStructuredBuffer<InstanceDrawData> instanceDrawingData : register(u1);
RWStructuredBuffer<InstanceData> instanceProperties : register(u2);
StructuredBuffer<SpawnData> spawnArgs : register(t0);

cbuffer cb0 : register(b0)
{
    float gametime;
    float deltaTime;
    uint spawnerCount;
    float pad;
};
