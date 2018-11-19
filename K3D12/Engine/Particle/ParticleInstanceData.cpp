
#include "ParticleInstanceData.h"
#include "../CommandContext/GraphicsCommandList.h"

constexpr unsigned int PARTICLE_MAX_NUM = 1024 * 10;

K3D12::ParticleInstanceData::ParticleInstanceData()
{
	Discard();
}

K3D12::ParticleInstanceData::~ParticleInstanceData()
{
}

void K3D12::ParticleInstanceData::Create(unsigned int spawnCountParFrame, unsigned int commandCountParFrame)
{

	this->_particleConstants._commandCount = commandCountParFrame;
	this->_particleConstants._spawnCount = spawnCountParFrame;
	this->_liveParticleNum = 0;

	this->_drawArgumentData.Create(sizeof(IndirectCommand), commandCountParFrame, nullptr);
	this->_particleCreateData.Create(sizeof(ParticleData), spawnCountParFrame, nullptr);
	this->_reservedIndexData.Create(sizeof(unsigned int),spawnCountParFrame,nullptr);
	this->_particleData.Create(sizeof(ParticleData), PARTICLE_MAX_NUM, nullptr);
	

}

void K3D12::ParticleInstanceData::AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties)
{
	this->_drawDescs.push_back(indirectPropaties);
}

void K3D12::ParticleInstanceData::SetDescriptorHeap(K3D12::GraphicsCommandList computeCommandList)
{
	ID3D12DescriptorHeap* heap[] = { this->_particleHeap.GetHeap().Get() };
	computeCommandList.GetCommandList()->SetDescriptorHeaps(1, heap);
}

void K3D12::ParticleInstanceData::Discard()
{
	this->_commandSignature.Discard();
	this->_drawDescs.clear();
	this->_drawDescs.shrink_to_fit();
	this->_particleCreateData.Discard();
	this->_particleData.Discard();
	this->_reservedIndexData.Discard();
	this->_particleHeap.Discard();
}
