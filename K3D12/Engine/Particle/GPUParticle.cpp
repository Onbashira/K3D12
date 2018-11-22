#include "GPUParticle.h"
#include "../Util/Utility.h"

K3D12::GPUParticle::GPUParticle()
{

}


K3D12::GPUParticle::~GPUParticle()
{
}


void K3D12::GPUParticle::Create(int particleMax, int emitNum)
{
	PARTICLE_MAX = (Util::Align(particleMax, THREAD_NUM_X));
	EMIT_NUM = (Util::Align(emitNum, THREAD_NUM_X));

	_spawanDataCount = 0;
	_gameTime = 0.0f;

	CreateBuffers();
	CreateRootSignature();
	CreateDescriptorHeap();
	CreatePipelineState();

	//ñ{èâä˙âª




}

void K3D12::GPUParticle::CreateBuffers()
{
}

void K3D12::GPUParticle::CreateRootSignature()
{
}

void K3D12::GPUParticle::CreateDescriptorHeap()
{
}

void K3D12::GPUParticle::CreatePipelineState()
{
}

