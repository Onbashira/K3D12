#include "MeshHeap.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../CommandContext/GraphicsCommandList.h"


K3D12::MeshHeap::MeshHeap() :
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

void K3D12::MeshHeap::SetHeapOffsetTransformStartIndex(unsigned int startIndex)
{
	this->_transformStartPoint = startIndex;
}

void K3D12::MeshHeap::SetHeapOffsetTextureStartIndex(unsigned int startIndex)
{
	this->_textureStartPoint = startIndex;
}

void K3D12::MeshHeap::SetHeapOffsetMaterialStartIndex(unsigned int startIndex)
{
	this->_materialStartPoint = startIndex;
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
