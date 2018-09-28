#pragma once
#include <vector>
#include <memory>
#include "../Resource/IndexBuffer.h"
#include "../Resource/VertexBuffer.h"
namespace K3D12 {
	//メッシュを付加するクラス
	template <class T>
	class MeshComponent
	{
	private:
		std::unique_ptr<IndexBuffer>	_indexBuffer;
		std::unique_ptr<VertexBuffer>	_vertexBuffer;

		std::vector<T>					_vertexes;
		std::vector<unsigned int>		_list;

	public:

	private:

	public:

		std::vector<T>& GetVertexData();
		std::vector<unsigned int>& GetIndexListData();
		IndexBuffer& GetIndexBuffer();
		VertexBuffer& GetVertexBuffer();

		K3D12::MeshComponent<T>& AddVertexData(const T& vertexData);
		K3D12::MeshComponent<T>& AddIndexListData(const unsigned int indexListParamater);

		void CreateIndexBuffer();
		void CreateVertexBuffer();


		MeshComponent();
		~MeshComponent();
	};

}

template<class T>
inline std::vector<T>&  K3D12::MeshComponent<T>::GetVertexData()
{
	this->_vertexes;
}

template<class T>
inline std::vector<unsigned int>&  K3D12::MeshComponent<T>::GetIndexListData()
{
	return this->_list
}

template<class T>
inline  K3D12::IndexBuffer &  K3D12::MeshComponent<T>::GetIndexBuffer()
{
	return *this->_indexBuffer;
}

template<class T>
inline  K3D12::VertexBuffer &  K3D12::MeshComponent<T>::GetVertexBuffer()
{
	return *this->_vertexBuffer;
}

template<class T>
inline K3D12::MeshComponent<T>& K3D12::MeshComponent<T>::AddIndexListData(const unsigned int indexListParamater)
{
	this->_list.emplace_back(indexListParamater);
	return *this;
}

template<class T>
inline K3D12::MeshComponent<T>& K3D12::MeshComponent<T>::AddVertexData(const T & vertexData)
{
	this->_list.emplace_back(vertexData);
	return *this;
}

template<class T>
inline void K3D12::MeshComponent<T>::CreateIndexBuffer()
{
	this->_indexBuffer->Create(_list.size() * sizeof(unsigned int), sizeof(unsigned int), &_list[0]);
}

template<class T>
inline void K3D12::MeshComponent<T>::CreateVertexBuffer()
{
	this->_vertexBuffer->Create(_vertexes.size() * sizeof(T), sizeof(T), &_vertexes[0]);
}

template<class T>
inline K3D12::MeshComponent<T>::MeshComponent() :
	_vertexBuffer(std::make_unique<K3D12::VertexBuffer>()),
	_indexBuffer(std::make_unique<K3D12::IndexBuffer>())
{
}

template<class T>
inline K3D12::MeshComponent<T>::~MeshComponent()
{
	_vertexBuffer.release();
	_indexBuffer.release();
}
