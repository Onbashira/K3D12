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

void K3D12::ModelMesh::DiscardMeshHeap()
{
	_meshHeap.GetHeap().Discard();
}
