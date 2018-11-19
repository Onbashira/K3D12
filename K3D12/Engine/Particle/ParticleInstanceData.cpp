#include "ParticleInstanceData.h"

void K3D12::ParticleInstanceData::Create(unsigned int spawnCountParFrame, unsigned int commandCountParFrame)
{
	this->_commandCount = commandCountParFrame;
	this->_spawnCount = spawnCountParFrame;


	
}

void K3D12::ParticleInstanceData::AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties)
{
	this->_drawDescs.push_back(indirectPropaties);
}
