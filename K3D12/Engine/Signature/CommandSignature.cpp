#include <string>
#include "CommandSignature.h"
#include "../Util/D3D12Common.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"
#include "RootSignature.h"

K3D12::CommandSignature::CommandSignature()  :
	_name("UnNamedCommandSignature")
{
}


K3D12::CommandSignature::~CommandSignature()
{
	Discard();
}


HRESULT K3D12::CommandSignature::Create(const D3D12_COMMAND_SIGNATURE_DESC & desc, RootSignature& rs)
{
	auto res = GET_DEVICE->CreateCommandSignature(&desc, rs.GetSignature().Get(), IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> K3D12::CommandSignature::GetSignature()
{
	return Microsoft::WRL::ComPtr<ID3D12RootSignature>();
}

void K3D12::CommandSignature::SetName(std::string name)
{
}

void K3D12::CommandSignature::Discard()
{
}


