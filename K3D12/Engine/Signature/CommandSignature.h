#pragma once
#include <string>
#include "../Util/D3D12Common.h"
#include "RootSignature.h"

namespace K3D12 {
	class CommandSignature
	{
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>	 _commandSignature;

		std::string										 _name;

	public:

		CommandSignature();

		~CommandSignature();

		HRESULT	Create(const D3D12_COMMAND_SIGNATURE_DESC& desc, RootSignature& rs);

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature();

		void	SetName(std::string name);

		void	Discard();

	};
}
