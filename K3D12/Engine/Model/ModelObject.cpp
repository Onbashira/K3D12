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
	this->DiscardMesh();
}

void K3D12::ModelObject::BindVertexBufferToBundle()
{
	auto vertexBuffers = GetMeshVBViews();
	_bundleList.GetCommandList()->IASetVertexBuffers(0, vertexBuffers.size(), &vertexBuffers[0]);
}

void K3D12::ModelObject::BindIndexBufferToBundle()
{
	_bundleList.GetCommandList()->IASetIndexBuffer(&this->GetDefaultIBO()->GetView());
}

void K3D12::ModelObject::InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData)
{
	this->GetCustomVBO()->Create(sizeof(Vertex3D) * defaultVertexData.size(), sizeof(Vertex3D), &defaultVertexData[0]);
}

void K3D12::ModelObject::InitializeDefaultIBO(std::vector<unsigned int>& defaultIndexListData)
{
	this->GetDefaultIBO()->Create(sizeof(unsigned int) * defaultIndexListData.size(), sizeof(unsigned int), &defaultIndexListData[0]);
}