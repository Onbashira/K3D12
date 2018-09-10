#pragma once
#include "../Util/D3D12Common.h"

namespace K3D12 {
	class GraphicsCommandList;
	class Fence;
	class CommandQueue
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_commandQueue;
	public:

	private:

	public:

		HRESULT												Create(D3D12_COMMAND_QUEUE_DESC desc);
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>			GetQueue()const;
		void												Wait(Fence* fence);
		UINT64												GetTimestampFrequency();
		D3D12_COMMAND_QUEUE_DESC							GetDesc();
		void												Discard();

		CommandQueue();
		~CommandQueue();
	};
}

