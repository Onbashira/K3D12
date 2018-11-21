#pragma once
#include "../../Engine/Util/D3D12Common.h"
#include "Resource.h"
#include "../../Engine/Util/Utility.h"

namespace K3D12 {
	template <typename T>
	class UploadBuffer : public Resource
	{
	private:

		unsigned int _elementByteSize = 0;
		bool isConstantBuffer = false;

	public:

	private:

	public:

		UploadBuffer(unsigned int elementCount = 1 ,bool isCB = false, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE) : 
			isConstantBuffer(isCB)
		{

			{
				this->_elementByteSize = sizeof(T);
				if (isConstantBuffer) {
					this->_elementByteSize = Util::Alignment256Bytes(_elementByteSize);
				}
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

			Create(prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
			Map(0, nullptr);


		};

		~UploadBuffer()
		{
			Unmap(0,nullptr);
		};

		void CopyData(unsigned int elementIndex, const T& data)
		{
			Read(data, sizeof(T), sizeof(T) * elementIndex);
		};

		void CopyArray(int arraySiz, const T* data)
		{
			Read(data, sizeof(T)*arraySiz, 0);

		};

		unsigned int ElementByteSize()
		{
			return _elementByteSize;
		}

		unsigned int ElementNum()
		{
			return _elementByteSize / sizeof(T);
		}
	};
}