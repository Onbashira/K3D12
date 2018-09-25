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
		rootPath.second->Rendering();
	}
}

std::weak_ptr<K3D12::RootRenderingPath> K3D12::SceneRenderer::AddRootRenderingPath(std::string name, RootRenderingPath rootRenderPath)
{
	if (this->_renderingPath.find(name) == _renderingPath.end()) {
		auto entity = std::make_shared<RootRenderingPath>(rootRenderPath);
		_renderingPath[name] = entity;
		return entity;
	}
	return std::weak_ptr<RootRenderingPath>();
}

std::weak_ptr<K3D12::RootRenderingPath> K3D12::SceneRenderer::GetRootRenderingPath(std::string name)
{
	if (this->_renderingPath.find(name) != _renderingPath.end()) {
		return _renderingPath[name];
	}
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

