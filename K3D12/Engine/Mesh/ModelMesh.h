#pragma once

#include <memory>
#include <vector>
#include <memory>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "MeshBuffer.h"
#include "MeshHeap.h"




namespace K3D12 {

	//モデルのメッシュ描画に必要なクラス
	class ModelMesh : public DrawableComponent
	{
	private:
		MeshHeap _meshHeap;

		MeshBuffer _meshBuffer;
	protected:

	public:

	private:

	protected:

		virtual void BindVertexBufferToBundle();

		virtual void BindIndexBufferToBundle();
		//バンドルリストに継承先でドローコールバッチ化作業を強制
		virtual void RegisterToBundle()override = 0;

	public:

		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;

		MeshHeap& GetMeshHeap();

		MeshBuffer& GetMeshBuffer();

		void BindDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list);

		void BindDescriptorHeaps(K3D12::GraphicsCommandList & list);

		void DiscardMeshHeap();

		void DiscardMeshBuffer();

		ModelMesh();
		virtual ~ModelMesh();
	};
};

