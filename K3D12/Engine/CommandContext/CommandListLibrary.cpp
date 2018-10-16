#include "CommandListLibrary.h"
#include "GraphicsCommandList.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Resource/DepthStencilBuffer.h"
#include "../Util/Logger.h"
#include "../Util/Utility.h"


K3D12::CommandListLibrary::CommandListLibrary()
{

}


K3D12::CommandListLibrary::~CommandListLibrary()
{
	Discard();
}


HRESULT K3D12::CommandListLibrary::Create(std::string commandListName,unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName] = std::make_shared<GraphicsCommandList>();
		auto hr = this->_library[commandListName]->Create(nodeMask, listType);
		this->_library[commandListName]->SetName(commandListName);
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string( "CommandList‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + commandListName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void K3D12::CommandListLibrary::Set(std::string commandListName, std::shared_ptr<GraphicsCommandList> commandList)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName].reset();
		this->_library[commandListName] = commandList;
		DEBUG_LOG(std::string("CommandList‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + commandListName));

	}
}

std::shared_ptr<K3D12::GraphicsCommandList> K3D12::CommandListLibrary::Get(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		return  this->_library[commandListName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + commandListName));

	return nullptr;
}

void K3D12::CommandListLibrary::Erase(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		this->_library.erase(commandListName);
		DEBUG_LOG(std::string("CommandList‚ª³í‚Éíœ‚³‚ê‚Ü‚µ‚½ : " + commandListName));

	}
}

void K3D12::CommandListLibrary::ConnectingPSO(std::string commandListName, PipelineStateObject& pso)
{
	auto hr = Get(commandListName)->ConnectingPSO(&pso);
	if (FAILED(hr)) {
		ERROR_LOG(std::string("CommandList‚ÆPSO‚ÌÚ‘±‚É¸”s : " + commandListName));

	}
}

void K3D12::CommandListLibrary::BindingDepthStencil(std::string commandListName, DepthStencil * depthStencil)
{
	Get(commandListName)->BindingDepthStencil(depthStencil);
}

void K3D12::CommandListLibrary::AddRenderTargets(std::string commandListName, D3D12_CPU_DESCRIPTOR_HANDLE handles[])
{
	Get(commandListName)->AddRenderTargets(handles);
}

void K3D12::CommandListLibrary::AddRenderTarget(std::string commandListName, D3D12_CPU_DESCRIPTOR_HANDLE handle, unsigned int index)
{
	Get(commandListName)->AddRenderTarget(handle, index);
}

void K3D12::CommandListLibrary::OMSetBindingRenderTarget(std::string commandListName)
{
	Get(commandListName)->OMSetBindingRenderTargets();
}

void K3D12::CommandListLibrary::OMSetRenderTargets(std::string commandListName,unsigned int numRenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[], bool RTsSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE *  dsvHandle)
{
	Get(commandListName)->OMSetRenderTargets(numRenderTargets, rtvHandles, RTsSingleHandleToDescriptorRange, dsvHandle);
}

void K3D12::CommandListLibrary::Discard()
{
	for (auto& list : _library) {
		list.second->Discard();
		list.second.reset();
	}
	_library.clear();
}

