#pragma once
#include "../Util/D3D12Common.h"

namespace K3D12 {
	class CommandAllocator
	{
	private:
	
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;

	public:

	private:

	public:
		
		CommandAllocator();
		
		~CommandAllocator();
		
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAllocator()const;
		
		void Discard();

	};
}

