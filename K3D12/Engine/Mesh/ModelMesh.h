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

		virtual void BindVertexBufferToBundle() = 0;

		virtual void BindIndexBufferToBundle() = 0;
		//バンドルリストに継承先でドローコールバッチ化作業を強制
		virtual void RegisterToBundle()override = 0;

		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData);

		virtual void InitializeCustomVBO(unsigned int size, unsigned int stride, void* customVertexDataSrc);

		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData);

	public:

		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;

		MeshHeap& GetMeshHeap();

		void BindDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list);

		void BindDescriptorHeaps(K3D12::GraphicsCommandList& list);

		void SetTransformDescStartIndex(unsigned int startIndex = 0);

		void SetTextureDescStartIndex(unsigned int startIndex = 1);

		void SetMaterialDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<K3D12::ShaderResource>>& GetTexturesRef();

		MeshHeap& AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef);

		void DiscardMeshHeap();

		ModelMesh();
		virtual ~ModelMesh();
	};
};

