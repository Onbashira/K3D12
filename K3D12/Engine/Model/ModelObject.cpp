#include "ModelObject.h"


K3D12::ModelObject::ModelObject()
{

}


K3D12::ModelObject::~ModelObject()
{

}

void K3D12::ModelObject::DiscardModelObject()
{
	this->DiscardMeshHeap();
	this->DiscardMesh();
}