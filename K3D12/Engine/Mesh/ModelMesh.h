#pragma once

#include <memory>
#include <vector>
#include <memory>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"




namespace K3D12 {

	class IndexBuffer;
	class VertexBuffer;
	class ConstantBuffer;
	class ShaderResource;

	//モデルのメッシュに必要な情報を持ったクラス	
	template <class VertexType>
	class ModelMesh : public K3D12::DrawableComponent, public MeshComponent<VertexType>,public MeshRenderer
	{
	private:
		
	protected:

	public:
		
	private:

	protected:

	public:
		

		const MeshComponent& GetMesh()const;
		const MeshRenderer&  GetMeshRenderer()const;

		ModelMesh();
		~ModelMesh();
	};
};

