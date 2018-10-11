#pragma once
#include "../Util/D3D12Common.h"
#include "Resource.h"

namespace K3D12 {

	class VertexBuffer : public K3D12::Resource
	{
	private:
		D3D12_VERTEX_BUFFER_VIEW _view;
		
		ULONG64 _size;
		
		UINT _stride;

	public:

	private:

		HRESULT	Create(D3D12_HEAP_PROPERTIES heapProps, D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clearValue = nullptr)override final { return E_FAIL; };

	public:		

		VertexBuffer();
		
		VertexBuffer(const VertexBuffer& other);
		
		VertexBuffer(VertexBuffer&& value);
		
		VertexBuffer& operator =(const VertexBuffer& value);
		
		VertexBuffer& operator =(VertexBuffer&& value);
		
		~VertexBuffer();
		
		virtual HRESULT Create(ULONG64 size, UINT stride, const void* pVertices);
		
		void Discard();

		D3D12_VERTEX_BUFFER_VIEW GetView() const;
		

	};
}