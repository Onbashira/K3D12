#include "StructuredBuffer.h"
#include "../System/D3D12System.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/CommandQueue.h"
#include "../AsyncComponent/Fence.h"
#include "../Util/Logger.h"

K3D12::StructuredBuffer::StructuredBuffer()
{
}


K3D12::StructuredBuffer::~StructuredBuffer()
{
	this->Discard();
}

HRESULT K3D12::StructuredBuffer::CreateHeap(unsigned numElements, unsigned int nodeMask)
{
	auto hr = this->_heap.Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numElements, nodeMask);
	_heap.SetName("UnorderedAccessValue_Heap");
	return hr;
}

HRESULT K3D12::StructuredBuffer::Create(unsigned int elementSize, unsigned int numElements, void* pBufferData)
{
	if (elementSize == 0 || numElements == 0) {
		return E_FAIL;
	}
#pragma region Initilaize UnorderdResource and StagingResource


	{
		D3D12_HEAP_PROPERTIES defaultHeapProp = {};
		D3D12_HEAP_PROPERTIES readBackHeapProp = {};

		D3D12_RESOURCE_DESC defaultResourceDesc = {};
		D3D12_RESOURCE_DESC readBackResourceDesc = {};
		{
			defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
			defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			defaultHeapProp.VisibleNodeMask = 1;
			defaultHeapProp.CreationNodeMask = 1;
			defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;

		}

		{

			readBackHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
			readBackHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			readBackHeapProp.VisibleNodeMask = 1;
			readBackHeapProp.CreationNodeMask = 1;
			readBackHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		}

		{
			defaultResourceDesc.Alignment = 0;
			defaultResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			defaultResourceDesc.Width = numElements * elementSize;
			defaultResourceDesc.Height = 1;
			defaultResourceDesc.DepthOrArraySize = 1;
			defaultResourceDesc.MipLevels = 1;
			defaultResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			defaultResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			defaultResourceDesc.SampleDesc.Count = 1;
			defaultResourceDesc.SampleDesc.Quality = 0;
			defaultResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		}
		{
			readBackResourceDesc.Alignment = 0;
			readBackResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			readBackResourceDesc.Width = numElements * elementSize;
			readBackResourceDesc.Height = 1;
			readBackResourceDesc.DepthOrArraySize = 1;
			readBackResourceDesc.MipLevels = 1;
			readBackResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			readBackResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			readBackResourceDesc.SampleDesc.Count = 1;
			readBackResourceDesc.SampleDesc.Quality = 0;
			readBackResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		}

		//���\�[�X�쐬
		{
			_readBackResource.Create(readBackHeapProp, D3D12_HEAP_FLAG_NONE,readBackResourceDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			Resource::Create(defaultHeapProp, D3D12_HEAP_FLAG_NONE, defaultResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
			SetName("UAVResource");
			//_stagingResource.SetName("UAVStagingResource");
#endif // _DEBUG

		}
	}
#pragma endregion
	//ReadRangeSetUp
	{
		_readRange.Begin = 0;
		_readRange.End = 1;
	}
	//this->WriteToBuffer(numElements, elementSize, pBufferData);

	this->Map(0, &this->_readRange);
	this->Update(pBufferData, numElements*elementSize, 0);
	this->Unmap(0, &this->_readRange);

	CHECK_RESULT(CreateHeap(HEAP_OFFSET::HEAP_OFFSET_MAX));
	CHECK_RESULT(CreateDescriptors(elementSize, numElements));

	return S_OK;
}

HRESULT K3D12::StructuredBuffer::CreateDescriptors(unsigned int elementSize, unsigned int numElements)
{
	//UAVStructuredView
	{
		D3D12_BUFFER_UAV uavBuffer;
		uavBuffer.FirstElement = 0;
		uavBuffer.NumElements = numElements;
		uavBuffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
		uavBuffer.CounterOffsetInBytes = 0;
		uavBuffer.StructureByteStride = elementSize;
		

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer = uavBuffer;

		this->CreateView(&uavDesc, _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET));
	}
	//SRVRawView
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv;
		srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		srv.Buffer.FirstElement = 0;
		srv.Buffer.NumElements = numElements;
		srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
		srv.Buffer.StructureByteStride = elementSize;
		this->CreateView(&srv, _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET));
	}
	return S_OK;

}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetSRVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetUAVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetSRVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::StructuredBuffer::GetUAVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

HRESULT K3D12::StructuredBuffer::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateUnorderedAccessView(_resource.Get(), _counterResource.Get(), uavDesc, cpuDescriptorHandle);
	return S_OK;
}

HRESULT K3D12::StructuredBuffer::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC * srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateShaderResourceView(_resource.Get(), srvDesc, cpuDescriptorHandle);
	return S_OK;
}

void K3D12::StructuredBuffer::WriteToBuffer(unsigned int numElements, unsigned int elementSize, void* pBufferData)
{

}

void K3D12::StructuredBuffer::ReadBack()
{
	ResourceTransition(D3D12System::GetCommandList("CommandList")->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
	D3D12System::GetCommandList("CommandList")->GetCommandList()->CopyResource(_readBackResource.GetResource(),this->_resource.Get());
	ResourceTransition(D3D12System::GetCommandList("CommandList")->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	//GPU�Ƃ̓��������A�����͌�ŃL���[�ƃ��X�g��I�����đҋ@�ł���悤�ɂ��Ȃ���΂Ȃ�Ȃ�
	{
		D3D12System::GetCommandList("CommandList")->CloseCommandList();

		ID3D12CommandList* lists[] = { D3D12System::GetCommandList("CommandList")->GetCommandList().Get() };

		D3D12System::GetInstance().GetMasterCommandQueue().GetQueue()->ExecuteCommandLists(1, lists);
		GET_SYSTEM_FENCE->Wait(&D3D12System::GetInstance().GetMasterCommandQueue());

		D3D12System::GetCommandList("CommandList")->ResetAllocator();
		D3D12System::GetCommandList("CommandList")->ResetCommandList();
	}
}

void K3D12::StructuredBuffer::Discard()
{
	_heap.Discard();
	_readBackResource.Discard();
	if (_counterResource.Get() != nullptr) {
		_counterResource.Reset();
	}
}

Microsoft::WRL::ComPtr<ID3D12Resource> K3D12::StructuredBuffer::GetCounterResource() const
{
	return _counterResource;
}

K3D12::DescriptorHeap * K3D12::StructuredBuffer::GetHeap()
{
	return &_heap;
}