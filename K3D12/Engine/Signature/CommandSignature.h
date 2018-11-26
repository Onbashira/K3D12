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

		//�R�}���h�o�b�t�@�̉��߂��L�q����DESC�ƃR�}���h�V�O�l�`����K�p����RootSignature�������Ɏ��
		HRESULT	Create(const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<K3D12::RootSignature> rs);

		//�R�}���h�o�b�t�@�̉��߂��L�q����DESC�ƃR�}���h�V�O�l�`����K�p����RootSignature�������Ɏ��
		HRESULT	Create(const D3D12_COMMAND_SIGNATURE_DESC& desc);

		Microsoft::WRL::ComPtr<ID3D12CommandSignature> GetSignature();

		void	SetName(std::string name);

		void	Discard();

	};
}
