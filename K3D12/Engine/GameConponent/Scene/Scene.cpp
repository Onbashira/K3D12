#include "Scene.h"

K3D12::Scene::Scene()
{
}

K3D12::Scene::~Scene()
{
}

void K3D12::Scene::SetName(std::string name)
{
	this->_sceneName = name;
}

std::string K3D12::Scene::GetName()
{
	return _sceneName;
}
