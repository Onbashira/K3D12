#pragma once
#include "../../Engine/Util/D3D12Common.h"
#include "../../Engine/System/D3D12System.h"
#include "Resource.h"
#include "../../Engine/Util/Utility.h"

namespace K3D12 {
	template <typename T>
	class UploadBuffer : public Resource
	{
	private:

		unsigned int _elementByteSize;
		bool isConstantBuffer;

	public:

	private:

	public:

		UploadBuffer() :
			_elementByteSize(0), isConstantBuffer(false)
		{

		};

		~UploadBuffer()
		{
			Unmap(0, nullptr);
		};

		void Create(unsigned int elementCount = 1, bool isCB = false, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE)
		{
			isConstantBuffer = isCB;

			this->_elementByteSize = sizeof(T);
			if (isConstantBuffer) {
				this->_elementByteSize = (unsigned int)Util::Alignment256Bytes(_elementByteSize);
			}


			D3D12_HEAP_PROPERTIES prop = {};
			D3D12_RESOURCE_DESC   desc = {};
			prop.Type = D3D12_HEAP_TYPE_UPLOAD;
			prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			prop.CreationNodeMask = 1;
			prop.VisibleNodeMask = 1;

			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Alignment = 0;
			desc.Width = _elementByteSize * elementCount;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = flags;

			Resource::Create(prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
			Map(0, nullptr);

		};

		void CopyData(unsigned int elementIndex, const T& data)
		{
			Resource::Update(&data, sizeof(T), sizeof(T) * elementIndex);
		};

		void CopyArray(int arraySiz, const T* data)
		{
			Resource::Update(&data, sizeof(T)*arraySiz, 0);
		};

		void CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cbvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
		{

			GET_DEVICE->CreateConstantBufferView(&cbvDesc, cpuDescriptorHandle);
		};

		void CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC & srv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
		{
			GET_DEVICE->CreateShaderResourceView(this->GetResource(), &srv, cpuDescriptorHandle);
		};

		void CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, ID3D12Resource* counterResource)
		{
			GET_DEVICE->CreateUnorderedAccessView(this->GetResource(), counterResource, &uav, cpuDescriptorHandle);
		};

		unsigned int ElementByteSize()
		{
			return _elementByteSize;
		};

		unsigned int ElementNum()
		{
			return _elementByteSize / sizeof(T);
		};

		T* Data() {
			return reinterpret_cast<T*>(GetMappedPointer());
		};
	};
};