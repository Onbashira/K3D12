#define PARTICLE_SPAWN
#include "ParticleCSStruct.hlsl"


[numthreads(256, 1, 1)]
void SpawnParticles(uint index : SV_DispatchThreadID)
{
	[branch]
    if (index < spawnerCount)
    {
        uint newIndex = reservedSlots.Consume();
        instanceProperties[newIndex].position = spawnArgs[index].position;
        instanceProperties[newIndex].forward = spawnArgs[index].forward;
        instanceProperties[newIndex].initialSpeedFactor = spawnArgs[index].initialSpeedFactor;
        instanceProperties[newIndex].reductionRate = spawnArgs[index].reductionRate;
        instanceProperties[newIndex].speedMag = spawnArgs[index].speedMag;
        instanceProperties[newIndex].lengthMag = spawnArgs[index].lengthMag;
        instanceProperties[newIndex].colorSamplingV = spawnArgs[index].colorSamplingV;
        instanceProperties[newIndex].spawnTime = gametime;
    }
}