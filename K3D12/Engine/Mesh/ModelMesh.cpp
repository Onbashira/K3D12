#include <memory>
#include "ModelMesh.h"


K3D12::ModelMesh::ModelMesh()
{
}


K3D12::ModelMesh::~ModelMesh()
{
}

K3D12::MeshHeap & K3D12::ModelMesh::GetMeshHeap()
{
	return _meshHeap;
}

void K3D12::ModelMesh::BindDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D12::ModelMesh::BindDescriptorHeaps(K3D12::GraphicsCommandList & list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D12::ModelMesh::SetTransformDescStartIndex(unsigned int startIndex)
{
	GetMeshHeap().SetTransformDescStartIndex(startIndex);
}

void K3D12::ModelMesh::SetTextureDescStartIndex(unsigned int startIndex)
{
	GetMeshHeap().SetTextureDescStartIndex(startIndex);
}

void K3D12::ModelMesh::SetMaterialDescStartIndex(unsigned int startIndex)
{
	GetMeshHeap().SetMaterialDescStartIndex(startIndex);
}

unsigned int K3D12::ModelMesh::GetTransformDescStartIndex()
{
	return GetMeshHeap().GetTransformDescStartIndex();
}

unsigned int K3D12::ModelMesh::GetTextureDescStartIndex()
{
	return GetMeshHeap().GetTextureDescStartIndex();
}

unsigned int K3D12::ModelMesh::GetMaterialDescStartIndex()
{
	return GetMeshHeap().GetMaterialDescStartIndex();
}

K3D12::ConstantBuffer & K3D12::ModelMesh::GetMaterialBufffer()
{
	return this->_meshHeap.GetMaterialBufffer();
}

std::vector<std::weak_ptr<K3D12::ShaderResource>>& K3D12::ModelMesh::GetTexturesRef()
{
	return this->GetMeshHeap().GetTexturesRef();
}

K3D12::MeshHeap & K3D12::ModelMesh::AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef)
{
	this->GetMeshHeap().AddTextureRef(textureRef);
}

void K3D12::ModelMesh::DiscardMeshHeap()
{
	_meshHeap.GetHeap().Discard();
}

