#include "SceneRenderer.h"
#include "RootRenderingPath.h"


K3D12::SceneRenderer::SceneRenderer()
{
}


K3D12::SceneRenderer::~SceneRenderer()
{
}


void K3D12::SceneRenderer::Rendering()
{
	int i = 0;
	for (auto& rootPath : this->_renderingPath) {
		if (rootPath.second->IsEnable()) {
			rootPath.second->Rendering();
		}
	}
}

std::shared_ptr<K3D12::RootRenderingPath> K3D12::SceneRenderer::AddRootRenderingPath(std::string name, RootRenderingPath rootRenderPath)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		return nullptr;
	}
	auto entity = std::make_shared<RootRenderingPath>(rootRenderPath);
	_renderingPath[name] = entity;
	return entity;
}

std::weak_ptr<K3D12::RootRenderingPath> K3D12::SceneRenderer::GetRootRenderingPath(std::string name)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		return _renderingPath[name];
	}
	return std::weak_ptr<K3D12::RootRenderingPath>();
}

void K3D12::SceneRenderer::EraseRenderingPath(std::string name)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		_renderingPath.erase(name);
	}
}

void K3D12::SceneRenderer::EnableRenderingPath(std::string name)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		_renderingPath[name]->Enable();
	}
}

void K3D12::SceneRenderer::DisableRenderingPath(std::string name)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		_renderingPath[name]->Disable();
	}
}


