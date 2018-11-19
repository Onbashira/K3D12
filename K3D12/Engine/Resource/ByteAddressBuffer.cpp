#include "ByteAddressBuffer.h"
#include "../System/D3D12System.h"

K3D12::ByteAddressBuffer::ByteAddressBuffer()
{
}

K3D12::ByteAddressBuffer::~ByteAddressBuffer()
{
	Discard();
}

HRESULT K3D12::ByteAddressBuffer::Create(unsigned int numElements)
{

	//byteOrder‚É‚·‚é
	D3D12_HEAP_PROPERTIES defaultHeapProp = {};
	D3D12_RESOURCE_DESC defaultResourceDesc = {};

	{
		defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
		defaultHeapProp.VisibleNodeMask = 1;
		defaultHeapProp.CreationNodeMask = 1;
		defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;

	}
	{
		defaultResourceDesc.Alignment = 0;
		defaultResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
		defaultResourceDesc.Width = numElements * sizeof(float);
		defaultResourceDesc.Height = 1;
		defaultResourceDesc.DepthOrArraySize = 1;
		defaultResourceDesc.MipLevels = 1;
		defaultResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		defaultResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		defaultResourceDesc.SampleDesc.Count = 1;
		defaultResourceDesc.SampleDesc.Quality = 0;
		defaultResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}
	Resource::Create(defaultHeapProp, D3D12_HEAP_FLAG_NONE, defaultResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
	SetName("ByteAddressBuffer");

#endif // _DEBUG


	return S_OK;
}

HRESULT K3D12::ByteAddressBuffer::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC * uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	//UAVStructuredView
	if (uavDesc == nullptr)
	{
		D3D12_BUFFER_UAV uavBuffer;
		uavBuffer.FirstElement = 0;
		uavBuffer.NumElements = _elementNum;
		uavBuffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_RAW;
		uavBuffer.CounterOffsetInBytes = 0;
		uavBuffer.StructureByteStride = _elementNum * sizeof(float);


		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer = uavBuffer;

		GET_DEVICE->CreateUnorderedAccessView(_resource.Get(), nullptr, &uavDesc, cpuDescriptorHandle);
		return S_OK;

	}
	GET_DEVICE->CreateUnorderedAccessView(_resource.Get(), nullptr, uavDesc, cpuDescriptorHandle);
	return S_OK;

}

HRESULT K3D12::ByteAddressBuffer::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC * srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	//SRVRawView
	if (srvDesc == nullptr)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv;
		srv.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
		srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		srv.Buffer.FirstElement = 0;
		srv.Buffer.NumElements = _elementNum;
		srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
		srv.Buffer.StructureByteStride = sizeof(unsigned int);
		GET_DEVICE->CreateShaderResourceView(_resource.Get(), &srv, cpuDescriptorHandle);
		return S_OK;
	}
	GET_DEVICE->CreateShaderResourceView(_resource.Get(), srvDesc, cpuDescriptorHandle);
	return S_OK;

}

void K3D12::ByteAddressBuffer::Discard()
{

}

Microsoft::WRL::ComPtr<ID3D12Resource> K3D12::ByteAddressBuffer::GetCounterResource() const
{
	return Microsoft::WRL::ComPtr<ID3D12Resource>();
}