#pragma once
#include <memory>


namespace K3D12 {

	class SceneRenderer;

	class RenderingPathManager
	{
	private:
		//シーン単位のレンダリングパイプライン
		std::shared_ptr<SceneRenderer> _sceneRenderer;
	public:
	private:
	public:
		//レンダリングパイプラインを破棄する
		void DiscardRenderingPipeline();
		//シーンレンダリングを設定する
		void SetSceneRenderer(std::shared_ptr<SceneRenderer> sceneRenderer);
		//レンダリングする
		void Rendering();
		RenderingPathManager();
		~RenderingPathManager();
	};
}

