#pragma once
#include <memory>
#include <vector>
#include "../Resource/ConstantBuffer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
namespace K3D12 {
	class ShaderResource;
	class GraphicsCommandList;

	//Meshをレンダリングするためのクラス。主にシェーダーに投げるためのヒープを管理する
	class MeshHeap
	{
	private:
	
		K3D12::DescriptorHeap								_heap;					//デスクリプタヒープ　					(変形情報 + マテリアル数 + テクスチャ数)　カメラの情報は別で投げる
	
		unsigned int										_transformStartPoint;	//変形情報デスクリプタの開始位置		(maybe 0)
	
		unsigned int										_materialStartPoint;	//マテリアルデスクリプタの開始位置		(maybe 0+1 ~ x)
		
		unsigned int										_textureStartPoint;		//テクスチャデスクリプタの開始位置		(maybe x+1 ~ y)

		K3D12::ConstantBuffer								_materialBuffer;		//マテリアル情報のバッファ 　　
	
		std::vector<std::weak_ptr<K3D12::ShaderResource>>	_textureResource;		//テクスチャリソースへの参照	
	public:

	protected:

	private:

	public:

		void BindingDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list);

		void BindingDescriptorHeaps(K3D12::GraphicsCommandList& lsit);

		void SetTransformDescStartIndex(unsigned int startIndex = 0);
		
		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		K3D12::DescriptorHeap& GetHeap();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<K3D12::ShaderResource>>& GetTexturesRef();

		std::weak_ptr<K3D12::ShaderResource> GetTexturteRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef);
		
		void Discard();
		
		MeshHeap();
		virtual ~MeshHeap();
	};
}

