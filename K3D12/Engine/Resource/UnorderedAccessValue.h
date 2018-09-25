#pragma once
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "Resource.h"

namespace K3D12 {

	class GraphcisCommandList;
	class CommandQueue;

	//UAVはDXGI_FORMATはUNKNWON固定
	class UnorderedAccessValue : public K3D12::Resource
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
		//CounterReosurce
		Microsoft::WRL::ComPtr<ID3D12Resource>	_counterResource;
		//StagingResource
		Resource								_stagingResource;
		//UploadResource
		Resource								_uploadResource;

		D3D12_RANGE								_cpuReadRange;

		DescriptorHeap							_heap;

		D3D12_UNORDERED_ACCESS_VIEW_DESC		_unorderedAccessViewDesc;
	public:
		
	private:

		HRESULT									CreateHeap(unsigned numElements, unsigned int nodeMask = 0);
		HRESULT									CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		HRESULT									CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		virtual HRESULT							Create(D3D12_HEAP_PROPERTIES heapProps, D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clearValue = nullptr) { return E_FAIL; };
	public:
		//リードバックバッファとアップロードバッファを作成。ついでにGPUに配置
		HRESULT									Create(unsigned int elementSize, unsigned int numElements, void* pBufferData);
		//デスクリプタの作成
		HRESULT									CreateDescriptors(unsigned int elementSize, unsigned int numElements);
		//リソースをアップロードバッファに配置
		void									WriteToBuffer(unsigned int numElements, unsigned int elementSize, void* pBufferData);
		//コマンドリストを指定してやること。非同期即時復帰でリソースのGPU配置操作を実行
		void									AsyncWriteToBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list,unsigned int numElements, unsigned int elementSize, void* pBufferData , K3D12::CommandQueue* queue = nullptr);
		//GPU上の情報をリードバックする
		void									ReadBack();
		//コマンドリストを指定してやること。非同期即時復帰でGPU上の情報をリードバック操作を実行
		void									AsyncReadBack(std::weak_ptr<K3D12::GraphicsCommandList> list, K3D12::CommandQueue* queue = nullptr);
		//
		D3D12_CPU_DESCRIPTOR_HANDLE				GetSRVCPUHandle();
		//
		D3D12_CPU_DESCRIPTOR_HANDLE				GetUAVCPUHandle();
		//
		D3D12_GPU_DESCRIPTOR_HANDLE				GetSRVGPUHandle();
		//
		D3D12_GPU_DESCRIPTOR_HANDLE				GetUAVGPUHandle();
		void									Discard();
		Microsoft::WRL::ComPtr<ID3D12Resource>	GetCounterResource()const;
		DescriptorHeap*							GetHeap();
		UnorderedAccessValue();
		virtual~UnorderedAccessValue();
	};
}