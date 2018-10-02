#include "IMesh.h"
#include "../Resource/VertexBuffer.h"



K3D12::IMesh::IMesh()
{

}


K3D12::IMesh::~IMesh()
{
	DiscardMesh();
}


std::vector<D3D12_VERTEX_BUFFER_VIEW> K3D12::IMesh::GetMeshVBViews()
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> meshVBViews;

	meshVBViews.push_back(_defaultVBO->GetView());
	if (this->_customVBO.get() != nullptr) {
		meshVBViews.push_back(_customVBO->GetView());
	}
	return meshVBViews;
}

void K3D12::IMesh::SetDefaultVBO(VertexBuffer * defaultVBO)
{
	this->_defaultVBO.reset(defaultVBO);
}

K3D12::VertexBuffer * K3D12::IMesh::GetDefaultVBO()
{
	return this->_defaultVBO.get();
}

void K3D12::IMesh::SetupCustomVBO(VertexBuffer * customVBO)
{
	this->_customVBO.reset(customVBO);
}

K3D12::VertexBuffer * K3D12::IMesh::GetCustomVBO()
{
	return this->_customVBO.get();
}

void K3D12::IMesh::SetDefaultIBO(IndexBuffer * defaultIBO)
{
	this->_defaultIBO.reset(defaultIBO);
}

K3D12::IndexBuffer * K3D12::IMesh::GetDefaultIBO()
{
	return _defaultIBO.get();
}

std::vector<Vertex3D>& K3D12::IMesh::GetDefaultVertexData()
{
	return this->_defaultVertexes;
}

std::vector<unsigned int>& K3D12::IMesh::GetDefaultIndexListData()
{
	return this->_defaultIndexList;
}

void K3D12::IMesh::DiscardMesh()
{
	this->_customVBO.reset();
	this->_defaultVBO.reset();
	this->_defaultIBO.reset();
	this->_defaultVertexes.clear();
	this->_defaultVertexes.shrink_to_fit();
	this->_defaultIndexList.clear();
	this->_defaultIndexList.shrink_to_fit();
}

bool K3D12::IMesh::UseCustomVertex()
{
	return  this->_customVBO.get() == nullptr ? false:true;
}