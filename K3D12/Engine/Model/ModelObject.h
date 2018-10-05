#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../Mesh/ModelMesh.h"
namespace K3D12 {
	//ベースのモデルオブジェクト　基本メッシュ情報、描画するためのヒープ、描画するためのパイプライン情報、ゲームオブジェクト属性を持つ
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:

	private:

	protected:
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle()override = 0;
		//バンドルにバインドする頂点バッファ
		virtual void BindVertexBufferToBundle()override;
		//バンドルにバインドするインデックスバッファ
		virtual void BindIndexBufferToBundle()override;

	public:
		//モデルのアップデート
		virtual void Update()override = 0;
		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;
		//デフォルトで使うVBOのセットアップ
		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData);
		//デフォルトで使うIBOのセットアップ
		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData);
		//ユーザー指定のVBOのセットアップ
		virtual void InitializeCustomVBO(void** customVertexDataSrc)override = 0;

		void DiscardModelObject();
		ModelObject();
		virtual ~ModelObject();
	};
}

