#include "MeshRenderer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../CommandContext/GraphicsCommandList.h"

K3D12::MeshRenderer::MeshRenderer() :
	_heap(std::make_unique< DescriptorHeap>()),
	_textureStartPoint(0),
	_materialStartPoint(0)
{

}


K3D12::MeshRenderer::~MeshRenderer()
{
	_heap->Discard();
	_heap.reset();
	_materialBuffer.Discard();
	_textureResource.clear();
}

void K3D12::MeshRenderer::BindingDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	if (!list.expired()) {
		ID3D12DescriptorHeap* heaps[] = { this->_heap->GetPtr() };

		list.lock()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	}
}

void K3D12::MeshRenderer::SetHeapOffsetTransformStartIndex(unsigned int startIndex)
{
	this->_transformStartPoint = startIndex;
}

void K3D12::MeshRenderer::SetHeapOffsetTextureStartIndex(unsigned int startIndex)
{
	this->_textureStartPoint = startIndex;
}

void K3D12::MeshRenderer::SetHeapOffsetMaterialStartIndex(unsigned int startIndex)
{
	this->_materialStartPoint = startIndex;
}

K3D12::ConstantBuffer & K3D12::MeshRenderer::GetMaterialBufffer()
{
	return this->_materialBuffer;
}

std::vector<std::weak_ptr<K3D12::ShaderResource>>& K3D12::MeshRenderer::GetTexturesRef()
{
	return this->_textureResource;
}

K3D12::MeshRenderer & K3D12::MeshRenderer::AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef)
{
	this->_textureResource.push_back(textureRef);
	return *this;
}


