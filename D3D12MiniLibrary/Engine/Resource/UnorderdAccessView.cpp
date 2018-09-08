#include "UnorderdAccessView.h"
#include "../System/D3D12System.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/CommandQueue.h"
#include "../AsyncComponent/Fence.h"
#include "../Util/Logger.h"

K3D12::UnorderedAccessValue::UnorderedAccessValue()
{
}


K3D12::UnorderedAccessValue::~UnorderedAccessValue()
{
	this->Discard();
}

HRESULT K3D12::UnorderedAccessValue::CreateHeap(unsigned numElements,  unsigned int nodeMask)
{
	auto hr = this->_heap.Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numElements, nodeMask);
	_heap.SetName("UnorderedAccessValue_Heap");
	return hr;
}

HRESULT K3D12::UnorderedAccessValue::Create(unsigned int elementSize, unsigned int numElements, void* pBufferData, UAV_MODE mode)
{
	if (elementSize == 0 || numElements == 0) {
		return E_FAIL;
	}
#pragma region Initilaize UnorderdResource and StagingResource
	D3D12_HEAP_PROPERTIES defaultHeapProp = {};
	D3D12_HEAP_PROPERTIES uploadHeapProp = {};

	D3D12_RESOURCE_DESC defaultResourceDesc;
	D3D12_RESOURCE_DESC uploadResourceDesc;
	Resource uploadResource;

	{
		defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		defaultHeapProp.VisibleNodeMask = 1;
		defaultHeapProp.CreationNodeMask = 1;
		defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	}
	{
		uploadHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		uploadHeapProp.VisibleNodeMask = 1;
		uploadHeapProp.CreationNodeMask = 1;
		uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	}
	{
		defaultResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numElements * elementSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	}
	{
		uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numElements * elementSize);
	}
	{//リソース作成

		_stagingResource.Create(defaultHeapProp, D3D12_HEAP_FLAG_NONE, defaultResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
		Resource::Create(uploadHeapProp, D3D12_HEAP_FLAG_NONE, uploadResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
#ifdef _DEBUG
		SetName("UAVUploadResource");
		_stagingResource.SetName("UAVStagingResource");
#endif // _DEBUG

	}
#pragma endregion

	this->WriteToBuffer(numElements, elementSize, pBufferData);

	CreateHeap(HEAP_OFFSET::HEAP_OFFSET_MAX);

	{ //UAVStructuredView
		D3D12_BUFFER_UAV uavBuffer;
		uavBuffer.FirstElement = 0;
		uavBuffer.NumElements = numElements;
		uavBuffer.Flags = (mode == UAV_MODE::STRUCTURED) ? D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE : D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_RAW;
		uavBuffer.CounterOffsetInBytes = 0;
		uavBuffer.StructureByteStride = elementSize;

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer = uavBuffer;

		this->CreateView(&uavDesc, _heap.GetCPUHandle(0));
	}
	{//SRVRawView
		D3D12_SHADER_RESOURCE_VIEW_DESC srv;
		srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		srv.Buffer.FirstElement = 0;
		srv.Buffer.NumElements = numElements;
		srv.Buffer.Flags = (mode == UAV_MODE::STRUCTURED) ? D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE : D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
		srv.Buffer.StructureByteStride = elementSize;
		this->CreateView(&srv, _heap.GetCPUHandle(1));
	}

	return S_OK;
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::UnorderedAccessValue::GetSRVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D12::UnorderedAccessValue::GetUAVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::UnorderedAccessValue::GetSRVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D12::UnorderedAccessValue::GetUAVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

HRESULT K3D12::UnorderedAccessValue::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateUnorderedAccessView(this->GetResource(), _counterResource.Get(), uavDesc, cpuDescriptorHandle);
	return S_OK;
}

HRESULT K3D12::UnorderedAccessValue::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC * srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	GET_DEVICE->CreateShaderResourceView(this->GetResource(), srvDesc, cpuDescriptorHandle);
	return S_OK;
}

void K3D12::UnorderedAccessValue::WriteToBuffer(unsigned int numElements,unsigned int elementSize, void* pBufferData)
{

	D3D12_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pData = pBufferData;
	subresourceData.RowPitch = numElements * elementSize;
	subresourceData.SlicePitch = 1;

	//ステージングのためのリソース遷移
	_stagingResource.ResourceTransition(D3D12System::GetCommandList("CommandList")->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	//リソース送信
	UpdateSubresources<1>(D3D12System::GetCommandList("CommandList")->GetCommandList().Get(),_stagingResource.GetResource(),_resource.Get(),0,0,1,&subresourceData);
	_stagingResource.ResourceTransition(D3D12System::GetCommandList("CommandList")->GetCommandList().Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	D3D12System::GetCommandList("CommandList")->CloseCommandList();

	ID3D12CommandList* lists[] = { D3D12System::GetCommandList("CommandList")->GetCommandList().Get() };

	D3D12System::GetInstance().GetCommandQueue()->GetQueue()->ExecuteCommandLists(1, lists);
	GET_SYSTEM_FENCE->Wait(D3D12System::GetInstance().GetCommandQueue());

	D3D12System::GetCommandList("CommandList")->ResetAllocator();
	D3D12System::GetCommandList("CommandList")->ResetCommandList();
}

void K3D12::UnorderedAccessValue::Discard()
{
	_heap.Discard();

	if (_counterResource.Get() != nullptr) {
		_counterResource.Reset();
	}
}

Microsoft::WRL::ComPtr<ID3D12Resource> K3D12::UnorderedAccessValue::GetCounterResource() const
{
	return _counterResource;
}

K3D12::DescriptorHeap * K3D12::UnorderedAccessValue::GetHeap()
{
	return &_heap;
}
