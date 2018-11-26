#pragma once
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "Resource.h"
#include "ByteAddressBuffer.h"

namespace K3D12 {

	class CommandQueue;

	//UAVはDXGI_FORMATはUNKNWON固定
	class StructuredBuffer : public Resource
	{
	public :
		enum BUFFER_MODE {
			UAV_BUFFER_MODE_APPEND,
			UAV_BUFFER_MODE_CONSUME

		};

	private:
		//Heapのオフセット値
		enum HEAP_OFFSET
		{
			UAV_DESCRIPTOR_OFFSET = 0,
			SRV_DESCRIPTOR_OFFSET = 1,
			HEAP_OFFSET_MAX,
		};
		//GPUMemにアップロードするためのメモリ　（リードバック用のメモリは継承元のリソース
		Resource								_readBackResource;

		D3D12_RANGE								_readRange;

		DescriptorHeap							_heap;

		D3D12_UNORDERED_ACCESS_VIEW_DESC		_unorderedAccessViewDesc;

		D3D12_SHADER_RESOURCE_VIEW_DESC			_shaderResourceViewDesc;
	public:
		
	private:

		HRESULT									CreateHeap(unsigned numElements, unsigned int nodeMask = 0);
		

	public:	

		StructuredBuffer();

		virtual~StructuredBuffer();
		//リードバックバッファとアップロードバッファを作成。ついでにGPUに配置
		HRESULT									Create(unsigned int elementSize, unsigned int numElements, void* pBufferData);
		//デスクリプタの作成
		HRESULT									CreateDescriptors(unsigned int elementSize, unsigned int numElements);

		HRESULT									CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle,Resource* counterResource = nullptr);

		HRESULT									CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void									WriteToBuffer(unsigned int numElements, unsigned int elementSize, void* pBufferData);

		void									WaitForProcess(K3D12::CommandQueue* queue);
		
		D3D12_CPU_DESCRIPTOR_HANDLE				GetSRVCPUHandle();
		
		D3D12_CPU_DESCRIPTOR_HANDLE				GetUAVCPUHandle();
		
		D3D12_GPU_DESCRIPTOR_HANDLE				GetSRVGPUHandle();
		
		D3D12_GPU_DESCRIPTOR_HANDLE				GetUAVGPUHandle();

		void									Discard();
				
		DescriptorHeap*							GetHeap();

		D3D12_UNORDERED_ACCESS_VIEW_DESC		GetUAVDesc();

		D3D12_SHADER_RESOURCE_VIEW_DESC			GetSRVDesc();

	};
}