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
		//�����_�����O�p�C�v���C����j������
		void DiscardRenderingPipeline();
		//�����_�����O�p�C�v���C�����}�l�[�W���郌���_���[�N���X���擾����
		std::weak_ptr<SceneRenderer> GetSceneRenderer();
		//�V�[�������_�����O��ݒ肷��
		void SetSceneRenderer(std::shared_ptr<SceneRenderer> sceneRenderer);
		//�����_�����O����
		void RenderingScene();
		RenderingPathManager();
		~RenderingPathManager();
	};
}

