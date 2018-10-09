#include <memory>
#include "ModelMesh.h"


K3D12::ModelMesh::ModelMesh()
{
}


K3D12::ModelMesh::~ModelMesh()
{
	DiscardMeshBuffer();
	DiscardMeshHeap();
}

void K3D12::ModelMesh::BindVertexBufferToBundle()
{
	auto vertexBuffers = this->_meshBuffer.GetMeshVBViews();
	_bundleList.GetCommandList()->IASetVertexBuffers(0, vertexBuffers.size(), &vertexBuffers[0]);
}

void K3D12::ModelMesh::BindIndexBufferToBundle()
{
	_bundleList.GetCommandList()->IASetIndexBuffer(&this->_meshBuffer.GetIBO().GetView());
}

K3D12::MeshHeap & K3D12::ModelMesh::GetMeshHeap()
{
	return _meshHeap;
}

K3D12::MeshBuffer & K3D12::ModelMesh::GetMeshBuffer()
{
	return _meshBuffer;
}

void K3D12::ModelMesh::BindDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D12::ModelMesh::BindDescriptorHeaps(K3D12::GraphicsCommandList & list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D12::ModelMesh::DiscardMeshHeap()
{
	_meshHeap.GetHeap().Discard();
}

void K3D12::ModelMesh::DiscardMeshBuffer()
{
	_meshBuffer.Discard();
}

