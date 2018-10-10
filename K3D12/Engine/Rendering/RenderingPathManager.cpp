#include "RenderingPathManager.h"
#include "SceneRenderer.h"
K3D12::RenderingPathManager::RenderingPathManager()
{
}

K3D12::RenderingPathManager::~RenderingPathManager()
{
	DiscardRenderingPipeline();
}


void K3D12::RenderingPathManager::DiscardRenderingPipeline()
{
	_sceneRenderer.reset();
}

std::weak_ptr<K3D12::SceneRenderer> K3D12::RenderingPathManager::GetSceneRenderer()
{
	return _sceneRenderer;
}

void K3D12::RenderingPathManager::SetSceneRenderer(std::shared_ptr<SceneRenderer> sceneRenderer)
{
	_sceneRenderer = sceneRenderer;
}

void K3D12::RenderingPathManager::RenderingScene()
{
	_sceneRenderer->Rendering();
}


