#pragma once
#include "../Util/D3D12Common.h"

namespace K3D12 {
	class Factory;

	class D3D12Device
	{
	private:

		Microsoft::WRL::ComPtr<ID3D12Device3>	_device;

		D3D_FEATURE_LEVEL						_featureLevel;

		bool									_useWarpDevice;
	public:

	private:

	public:	

		D3D12Device();

		~D3D12Device();

		HRESULT									Create(Factory* factory, bool useWarpDevice = false);

		Microsoft::WRL::ComPtr<ID3D12Device3>	GetDevice()const;

		const D3D_FEATURE_LEVEL&				GetFeatureLevel()const;

		void									Discard();

	};

};