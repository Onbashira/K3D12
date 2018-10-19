#include "AudioSourceManager.h"


void K3D12::AudioSourceManager::SetResource(std::string resourceName,std::shared_ptr<AudioWaveSource> source)
{
	this->_resourceMap.Set(resourceName,source);
}

std::weak_ptr<K3D12::AudioWaveSource> K3D12::AudioSourceManager::GetResource(std::string resourceName)
{
	return this->_resourceMap.Get(resourceName);
}

void K3D12::AudioSourceManager::EraseResource(std::string name)
{
	this->_resourceMap.Erase(name);
}

bool K3D12::AudioSourceManager::IsLoaded(std::string resourceName)
{
	return this->_resourceMap.IsLooded(resourceName);
}

void K3D12::AudioSourceManager::DiscardManager()
{
	this->_resourceMap.DiscardMap();
}
