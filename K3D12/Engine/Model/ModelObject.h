#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../Mesh/ModelMesh.h"
namespace K3D12 {
	//ベースのモデルオブジェクト　基本メッシュ情報、描画するためのヒープ、描画するためのパイプライン情報、ゲームオブジェクト属性を持つ
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:

	private:

	public:
		//モデルのアップデート
		virtual void Update() = 0;
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle() = 0;
		//描画時呼び出し関数の作成を強制
		virtual void DrawModel() = 0;
		//デフォルトで使うVBOのセットアップ
		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData) = 0;
		virtual void InitializeCustomVBO(void** customVertexDataSrc) = 0;
		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData) = 0;
		virtual void BindingVertexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;
		virtual void BindingIndexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;

		void DiscardModelObject();
		ModelObject();
		virtual ~ModelObject();
	};
}

