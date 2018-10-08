#include "MeshBuffer.h"



K3D12::MeshBuffer::MeshBuffer()
{

}


K3D12::MeshBuffer::~MeshBuffer()
{
	Discard();
}


void K3D12::MeshBuffer::InitializeVBO(ULONG64 size, unsigned int stride, void* vertexDataSrc)
{
	_vertexBuffer.Create(size, stride, vertexDataSrc);
}

void K3D12::MeshBuffer::AddCustomVBO(ULONG64 size, unsigned int stride, void * customVertexDataSrc)
{
	_customVBOs.push_back(K3D12::VertexBuffer());
	_customVBOs.back().Create(size, stride, customVertexDataSrc);
}

void K3D12::MeshBuffer::InitializeIBO(std::vector<unsigned int>& indexListDataSrc)
{
	_indexBuffer.Create(sizeof(unsigned int)*indexListDataSrc.size(), sizeof(unsigned int), &indexListDataSrc[0]);
}

std::vector<D3D12_VERTEX_BUFFER_VIEW> K3D12::MeshBuffer::GetMeshVBViews()
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> meshVBViews;

	meshVBViews.push_back(_vertexBuffer.GetView());
	if (this->_customVBOs.size() != 0) {
		for (auto& vb : _customVBOs) {
			meshVBViews.push_back(vb.GetView());
		}
	}
	return meshVBViews;
}

K3D12::VertexBuffer& K3D12::MeshBuffer::GetVBO()
{
	return _vertexBuffer;
}

K3D12::VertexBuffer* K3D12::MeshBuffer::GetCustomVBO(unsigned int index)
{
	if (this->_customVBOs.size() == 0 || (index >= _customVBOs.size())) {
		return nullptr;
	}
	return &_customVBOs[index];

}

K3D12::IndexBuffer& K3D12::MeshBuffer::GetIBO()
{
	return _indexBuffer;
}

void K3D12::MeshBuffer::Discard()
{
	this->_customVBOs.clear();
	this->_vertexBuffer.Discard();
	this->_indexBuffer.Discard();

}

bool K3D12::MeshBuffer::UseCustomVertex()
{
	return  this->_customVBOs.size() == 0 ? false : true;
}