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


HRESULT K3D12::CommandSignature::Create(const D3D12_COMMAND_SIGNATURE_DESC & desc, std::weak_ptr<K3D12::RootSignature> rs)
{
	auto res = GET_DEVICE->CreateCommandSignature(&desc, rs.lock()->GetSignature().Get(), IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

HRESULT K3D12::CommandSignature::Create(const D3D12_COMMAND_SIGNATURE_DESC & desc)
{
	auto res = GET_DEVICE->CreateCommandSignature(&desc, nullptr, IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

Microsoft::WRL::ComPtr<ID3D12CommandSignature> K3D12::CommandSignature::GetSignature()
{
	return this->_commandSignature;
}

void K3D12::CommandSignature::SetName(std::string name)
{
	if (_commandSignature.Get() != nullptr) {
		this->_commandSignature->SetName(Util::StringToWString(name).c_str());
		_name = name;
	}
}

void K3D12::CommandSignature::Discard()
{
	if (_commandSignature.Get() != nullptr) {
		_commandSignature.Reset();
		DEBUG_LOG(std::string("CommandSignature : " + _name + " is  Reset"));
	}
}


