#include "DrawableComponent.h"
#include "../Signature/RootSignature.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../PipelineState/PipelineStateObject.h"

K3D12::DrawableComponent::DrawableComponent()  :
	_commandList()
{
	_bundleList.Create(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
}


K3D12::DrawableComponent::~DrawableComponent()
{
	this->_commandList.reset();
	this->_rootSignature.reset();
	this->_pipelineState.reset();
	this->_bundleList.Discard();
}


void K3D12::DrawableComponent::BindingShaderObject()
{
	_commandList.lock()->GetCommandList()->SetPipelineState(_pipelineState.lock()->GetPSO().Get());
	_commandList.lock()->GetCommandList()->SetGraphicsRootSignature(_rootSignature.lock()->GetSignature().Get());
}

void K3D12::DrawableComponent::BindingShaderObjectToBundle()
{
	_bundleList.GetCommandList()->SetPipelineState(_pipelineState.lock()->GetPSO().Get());
	_bundleList.GetCommandList()->SetGraphicsRootSignature(_rootSignature.lock()->GetSignature().Get());
}

void K3D12::DrawableComponent::SetPipelineState(std::weak_ptr<K3D12::PipelineStateObject> pipelineState)
{
	_pipelineState = pipelineState;
}

void K3D12::DrawableComponent::SetRootSignature(std::weak_ptr<K3D12::RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
}

void K3D12::DrawableComponent::SetMasterCommandList(std::shared_ptr<GraphicsCommandList> masterCommandList)
{
	_commandList = masterCommandList;
}
