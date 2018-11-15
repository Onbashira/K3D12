#pragma once
#include <vector>
#include "../Util/Math.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "../Mesh/ModelMesh.h"
namespace K3D12 {
	//ベースのモデルオブジェクト　基本メッシュ情報、描画するためのヒープ、描画するためのパイプライン情報、ゲームオブジェクト属性を持つ
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:
		std::vector<Vertex3D> _vertexes;

		std::vector<unsigned int> _indexList;
	private:

	protected:
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle()override = 0;
	public:
		//モデルのアップデート
		virtual void Update()override = 0;
		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;

		void DiscardModelObject();

		ModelObject();
		virtual ~ModelObject();
	};
}

