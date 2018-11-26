#include <d3d12.h>
#include "ShaderObjectLibrary.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Signature/RootSignature.h"
#include "../Util/Logger.h"
#include "../Util/Utility.h"


K3D12::ShaderObjectLibrary::ShaderObjectLibrary()
{
}


K3D12::ShaderObjectLibrary::~ShaderObjectLibrary()
{
	Discard();
}


HRESULT K3D12::ShaderObjectLibrary::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		HRESULT hr = {};
		this->_psolibrary[psoName] = std::make_shared<K3D12::PipelineStateObject>();
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName, signature);
			hr = this->_psolibrary[psoName]->Create(psoName, gps, GetRootSignature(psoName ));

		}
		else {
			hr = this->_psolibrary[psoName]->Create(psoName, gps, nullptr);
		}
		if (SUCCEEDED(hr)) {

			DEBUG_LOG(std::string("PSO‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + psoName ));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D12::ShaderObjectLibrary::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC cps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = std::make_shared<K3D12::PipelineStateObject>();
		HRESULT hr = {};
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName + "RootSignature", signature);
			hr = this->_psolibrary[psoName]->Create(psoName, cps, GetRootSignature(psoName + "RootSignature"));

		}
		else {
			hr = this->_psolibrary[psoName]->Create(psoName, cps, nullptr);
		}
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("PSO‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + psoName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D12::ShaderObjectLibrary::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<K3D12::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromShader(signature);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D12::ShaderObjectLibrary::CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC * desc)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<K3D12::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromDesc(desc);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void K3D12::ShaderObjectLibrary::SetPSO(std::string psoName, std::shared_ptr<K3D12::PipelineStateObject> pso)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = pso;
		DEBUG_LOG(std::string("PSO‚ğ“o˜^‚µ‚Ü‚µ‚½ : " + psoName));

	}
}

void K3D12::ShaderObjectLibrary::SetRootSignature(std::string rsName, std::shared_ptr<K3D12::RootSignature> rootSignature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = rootSignature;
		DEBUG_LOG(std::string("RootSignature‚ğ“o˜^‚µ‚Ü‚µ‚½ : " + rsName ));
	}
}

std::weak_ptr<K3D12::PipelineStateObject> K3D12::ShaderObjectLibrary::GetPSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		return  this->_psolibrary[psoName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + psoName ));

	return std::weak_ptr<K3D12::PipelineStateObject>();
}

std::weak_ptr<K3D12::RootSignature> K3D12::ShaderObjectLibrary::GetRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		return  this->_rootSignaturelibrary[rsName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + rsName));

	return std::weak_ptr<K3D12::RootSignature>();
}

void K3D12::ShaderObjectLibrary::ErasePSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		this->_psolibrary.erase(psoName);
		DEBUG_LOG(std::string("PSO‚ğíœ‚µ‚Ü‚µ‚½ : " + psoName));

	}
}

void K3D12::ShaderObjectLibrary::EraseRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary.erase(rsName);
		DEBUG_LOG(std::string("RootSignature‚ğíœ‚µ‚Ü‚µ‚½ : " + rsName));

	}
}

void K3D12::ShaderObjectLibrary::Discard()
{

	this->_psolibrary.clear();
	this->_rootSignaturelibrary.clear();
}
