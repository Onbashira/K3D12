#pragma once
#include <string>
#include "../Util/D3D12Common.h"

namespace K3D12 {
	class DescriptorHeap
	{
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuHandle;
		UINT											_type;
		UINT											_incrementSize;
		D3D12_DESCRIPTOR_HEAP_DESC						_heapDesc;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;
		std::string										_heapName;
	public:

	private:

	public:
		DescriptorHeap();

		~DescriptorHeap();

		D3D12_CPU_DESCRIPTOR_HANDLE						GetCPUHandle(UINT indexCount)const;

		D3D12_GPU_DESCRIPTOR_HANDLE						GetGPUHandle(UINT indexCount)const;

		UINT											GetIncrementSize()const;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	GetHeap()const;

		ID3D12DescriptorHeap* 							GetPtr()const;

		const D3D12_DESCRIPTOR_HEAP_DESC&				GetDesc()const;

		ID3D12DescriptorHeap*							operator ->();

		HRESULT											Create(D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		HRESULT											Create(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		void											SetName(std::string name);

		void											Discard();

	};

}
