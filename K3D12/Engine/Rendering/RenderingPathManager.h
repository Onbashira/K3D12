#pragma once
#include <memory>


namespace K3D12 {

	class SceneRenderer;

	class RenderingPathManager
	{
	private:
		//�V�[���P�ʂ̃����_�����O�p�C�v���C��
		std::shared_ptr<SceneRenderer> _sceneRenderer;
	public:
	private:
	public:
		void DiscardRenderingPipeline();
		void SetSceneRenderer(std::shared_ptr<SceneRenderer> sceneRenderer);
		void Rendering();
		RenderingPathManager();
		~RenderingPathManager();
	};
}

