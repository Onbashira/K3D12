
#include "ParticleCSStruct.hlsl"


void UpdateParticle(inout InstanceData instanceRef, float lifetime)
{
    const float currentSpeed = instanceRef.initialSpeedFactor * pow(abs(instanceRef.reductionRate), lifetime * 60.0f);
    instanceRef.position = mad(float4(instanceRef.forward, 0.0f), currentSpeed * instanceRef.speedMag * 3 * deltaTime, instanceRef.position);
    const float4 pos2 = mad(float4(instanceRef.forward, 0.0f), (currentSpeed + 0.0625f) * instanceRef.lengthMag / 10.0f, instanceRef.position);
    InstanceDrawData draw;
    draw.pad = float2(0.0f, 0.0f);
    draw.position[0] = instanceRef.position;
    draw.position[1] = pos2;
    draw.colorSampleCoord = float2(lifetime, instanceRef.colorSamplingV);
    instanceDrawingData.Append(draw);
}

[numthreads(1024, 1, 1)]
void UpdateParticles(uint index : SV_DispatchThreadID)
{
	[branch]
    if (instanceProperties[index].position.w > 0.0f)
    {

        const float lifetime = gametime + deltaTime - instanceProperties[index].spawnTime;
		[branch]
        if (lifetime < 1.0f)
        {
            UpdateParticle(instanceProperties[index], lifetime);
        }
        else
        {
            instanceProperties[index].position.w = 0.0f;
            reservedSlotsApp.Append(index);
        }
    }

}