#include "ModelObject.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../Resource/VertexBuffer.h"
#include "../Resource/IndexBuffer.h"

K3D12::ModelObject::ModelObject()
{

}


K3D12::ModelObject::~ModelObject()
{
	DiscardModelObject();
}

void K3D12::ModelObject::DiscardModelObject()
{
	this->DiscardMeshHeap();
	this->DiscardMeshBuffer();
}