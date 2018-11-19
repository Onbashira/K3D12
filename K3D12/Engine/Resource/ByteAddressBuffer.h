#pragma once
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "Resource.h"

namespace K3D12 {
	class ByteAddressBuffer : public Resource
	{
	private:
		//CounterResource
		D3D12_UNORDERED_ACCESS_VIEW_DESC		_unorderedAccessViewDesc;

		unsigned int							_elementNum;
	public:

	private:

	public:

		ByteAddressBuffer();

		virtual ~ByteAddressBuffer();

		HRESULT									Create(unsigned int numElements = 1);

		HRESULT									CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		HRESULT									CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void									Discard();

		Microsoft::WRL::ComPtr<ID3D12Resource>	GetCounterResource()const;

	};
}
