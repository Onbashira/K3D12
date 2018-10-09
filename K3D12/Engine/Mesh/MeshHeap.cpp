#include "MeshHeap.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../CommandContext/GraphicsCommandList.h"


K3D12::MeshHeap::MeshHeap()  :
	_heap(),
	_textureStartPoint(0),
	_materialStartPoint(0)
{
}


K3D12::MeshHeap::~MeshHeap()
{
	Discard();
}


void K3D12::MeshHeap::BindingDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	if (!list.expired()) {
		ID3D12DescriptorHeap* heaps[] = { this->_heap.GetPtr() };

		list.lock()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	}
}

void K3D12::MeshHeap::BindingDescriptorHeaps(K3D12::GraphicsCommandList & lsit)
{
	ID3D12DescriptorHeap* heaps[] = { this->_heap.GetPtr() };

	lsit.GetCommandList()->SetDescriptorHeaps(1, heaps);

}

void K3D12::MeshHeap::SetTransformDescStartIndex(unsigned int startIndex)
{
	this->_transformStartPoint = startIndex;

}

void K3D12::MeshHeap::SetMaterialDescStartIndex(unsigned int startIndex)
{
	this->_materialStartPoint = startIndex;

}

void K3D12::MeshHeap::SetTextureDescStartIndex(unsigned int startIndex)
{
	this->_textureStartPoint = startIndex;

}

unsigned int K3D12::MeshHeap::GetTransformDescStartIndex()
{
	return _transformStartPoint;
}

unsigned int K3D12::MeshHeap::GetTextureDescStartIndex()
{
	return _textureStartPoint;
}

unsigned int K3D12::MeshHeap::GetMaterialDescStartIndex()
{
	return _materialStartPoint;
}

K3D12::DescriptorHeap & K3D12::MeshHeap::GetHeap()
{
	return _heap;
}

K3D12::ConstantBuffer & K3D12::MeshHeap::GetMaterialBufffer()
{
	return this->_materialBuffer;
}

std::vector<std::weak_ptr<K3D12::ShaderResource>>& K3D12::MeshHeap::GetTexturesRef()
{
	return this->_textureResource;
}

std::weak_ptr<K3D12::ShaderResource> K3D12::MeshHeap::GetTexturteRef(unsigned int textureIndex)
{
	if (_textureResource.size() == 0 || _textureResource.size() <= textureIndex) {
		return std::weak_ptr<K3D12::ShaderResource>();
	}
	return _textureResource[textureIndex];
}

K3D12::MeshHeap & K3D12::MeshHeap::AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef)
{
	this->_textureResource.push_back(textureRef);
	return *this;
}


void K3D12::MeshHeap::Discard()
{
	_heap.Discard();
	_materialBuffer.Discard();
	_textureResource.clear();
	_textureResource.shrink_to_fit();
}
