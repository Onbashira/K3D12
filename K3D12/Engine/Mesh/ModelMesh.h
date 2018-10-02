#pragma once

#include <memory>
#include <vector>
#include <memory>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "IMesh.h"
#include "MeshHeap.h"




namespace K3D12 {

	//モデルのメッシュ描画に必要なクラス
	class ModelMesh : public DrawableComponent, public IMesh
	{
	private:
		MeshHeap _meshHeap;
	protected:
	public:

	private:

		virtual void BindVertexBufferToBundle()override = 0;

		virtual void BindIndexBufferToBundle()override = 0;
		//バンドルリストに継承先でドローコールバッチ化作業を強制
		virtual void RegisterToBundle()override = 0;

	protected:

	public:


		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;

		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData)override = 0;

		virtual void InitializeCustomVBO(void** customVertexDataSrc)override = 0;

		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData)override = 0;

		MeshHeap& GetMeshHeap();
		
		void DiscardMeshHeap();

		ModelMesh();
		virtual ~ModelMesh();
	};
};

