#pragma once
#include <vector>
#include <memory>
#include "../Resource/IndexBuffer.h"
#include "../Resource/VertexBuffer.h"
#include "../CommandContext/GraphicsCommandList.h"
namespace K3D12 {
	class GraphcisCommandList;
	//メッシュを付加するクラス
	template <class T >
	class MeshComponent
	{
	private:
		//SysMemに確保される頂点インデックスリスト
		std::unique_ptr<IndexBuffer>	_indexBuffer;
		//SysMemに確保される頂点情報
		std::unique_ptr<VertexBuffer>	_vertexBuffer;

		//頂点情報本体
		std::vector<T>					_vertexes;
		//インデックスリスト本体
		std::vector<unsigned int>		_list;
		//頂点毎の入力レイアウト
		std::vector<D3D12_INPUT_ELEMENT_DESC> _vertexInputElement;

	public:

	private:

	public:

		std::vector<T>& GetVertexData();
		std::vector<unsigned int>& GetIndexListData();
		IndexBuffer& GetIndexBuffer();
		VertexBuffer& GetVertexBuffer();
		std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputElementDescs();

		K3D12::MeshComponent<T>& AddVertexData(const T& vertexData);
		K3D12::MeshComponent<T>& AddIndexListData(const unsigned int indexListParamater);
		K3D12::MeshComponent<T>& AddVertexInputElement(const D3D12_INPUT_ELEMENT_DESC& elementDesc);

		void CreateIndexBuffer();
		void CreateVertexBuffer();

		void BindingVertexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list);
		void BindingIndexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list);

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
inline std::vector<D3D12_INPUT_ELEMENT_DESC> & K3D12::MeshComponent<T>::GetInputElementDescs()
{
	return _vertexInputElement;
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
inline K3D12::MeshComponent<T>& K3D12::MeshComponent<T>::AddVertexInputElement(const D3D12_INPUT_ELEMENT_DESC & elementDesc)
{
	this->_list.emplace_back(elementDesc);
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
inline void K3D12::MeshComponent<T>::BindingVertexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	list.lock()->GetCommandList()->IASetVertexBuffers(0, 1, &this->_vertexBuffer->GetView());
}

template<class T>
inline void K3D12::MeshComponent<T>::BindingIndexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list)
{
	list.lock()->GetCommandList()->IASetIndexBuffer(&_indexBuffer->GetView());
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
