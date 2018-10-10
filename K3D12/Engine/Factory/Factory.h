#pragma once
#include"../Util/D3D12Common.h"


namespace K3D12 {
	class Factory
	{
	private:
		Microsoft::WRL::ComPtr<IDXGIFactory6>	_factory;
	public:

	private:

	public:

		Factory();
		
		~Factory();
		
		HRESULT	Create(UINT createFlags = 0);
	
		Microsoft::WRL::ComPtr<IDXGIFactory6> GetFactory()const;
	
		void Discard();
	};

}