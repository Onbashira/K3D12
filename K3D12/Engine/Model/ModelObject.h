#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/MeshRenderer.h"
namespace K3D12 {
	template <class T>
	//ベースのモデルオブジェクト　基本メッシュ情報、描画するためのヒープ、描画するためのパイプライン情報、ゲームオブジェクト属性を持つ
	class ModelObject : public GameObject ,public DrawableComponent,public MeshComponent<T>,public MeshRenderer
	{
	private:

	public:

	private:

	public:
		ModelObject();
		virtual ~ModelObject();
	};
}

