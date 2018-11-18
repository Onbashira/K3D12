#pragma once
#include "Resource.h"

namespace K3D12 {
	class CommandBuffer : public Resource
	{
	private:
		D3D12_GPU_DESCRIPTOR_HANDLE _commandHandle;
		unsigned int _bufferSize;

	public:

	private:

	public:
		CommandBuffer();

		virtual ~CommandBuffer();

		void Create(unsigned int BufferSize);


	};
}