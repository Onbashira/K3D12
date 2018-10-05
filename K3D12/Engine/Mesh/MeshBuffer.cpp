#include "MeshBuffer.h"
#include "../Resource/VertexBuffer.h"



K3D12::MeshBuffer::MeshBuffer()
{

}


K3D12::MeshBuffer::~MeshBuffer()
{
	DiscardMesh();
}


void K3D12::MeshBuffer::InitializeDefaultVBO(ULONG64 size, unsigned int stride, void* defaultVertexDataSrc)
{
	_defaultVBO->Create(size, stride, defaultVertexDataSrc);
}

void K3D12::MeshBuffer::InitializeCustomVBO(ULONG64 size, unsigned int stride, void * customVertexDataSrc)
{
	_customVBO->Create(size, stride, customVertexDataSrc);

}

void K3D12::MeshBuffer::InitializeIBO(std::vector<unsigned int>& defaultIndexListData)
{
	_defaultIBO->Create(sizeof(unsigned int)*defaultIndexListData.size(),sizeof(unsigned int),  &defaultIndexListData[0]);
}

std::vector<D3D12_VERTEX_BUFFER_VIEW> K3D12::MeshBuffer::GetMeshVBViews()
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> meshVBViews;

	meshVBViews.push_back(_defaultVBO->GetView());
	if (this->_customVBO.get() != nullptr) {
		meshVBViews.push_back(_customVBO->GetView());
	}
	return meshVBViews;
}

K3D12::VertexBuffer * K3D12::MeshBuffer::GetDefaultVBO()
{
	return this->_defaultVBO.get();
}

K3D12::VertexBuffer * K3D12::MeshBuffer::GetCustomVBO()
{
	return this->_customVBO.get();
}

K3D12::IndexBuffer * K3D12::MeshBuffer::GetIBO()
{
	return _defaultIBO.get();
}

void K3D12::MeshBuffer::DiscardMesh()
{
	this->_customVBO.reset();
	this->_defaultVBO.reset();
	this->_defaultIBO.reset();
	//this->_defaultVertexes.clear();
	//this->_defaultVertexes.shrink_to_fit();
	//this->_defaultIndexList.clear();
	//this->_defaultIndexList.shrink_to_fit();
}

bool K3D12::MeshBuffer::UseCustomVertex()
{
	return  this->_customVBO.get() == nullptr ? false:true;
}