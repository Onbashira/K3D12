#include "RenderingPathBase.h"


K3D12::RenderingPathBase::RenderingPathBase()
{
}


K3D12::RenderingPathBase::~RenderingPathBase()
{
	_commandList.reset();
}


void K3D12::RenderingPathBase::InitBeforExcution()
{
	this->_onInitBE(this->_commandList);
}

void K3D12::RenderingPathBase::InitAfterExcution()
{
	this->_onInitAE(this->_commandList);
}

void K3D12::RenderingPathBase::InitFrameStart()
{
	this->_onInitFS(this->_commandList);
}

void K3D12::RenderingPathBase::SetBeforExcutionInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)> function)
{
	this->_onInitBE = function;
}

void K3D12::RenderingPathBase::SetAfterExcutionInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)> function)
{
	this->_onInitAE = function;
}

void K3D12::RenderingPathBase::SetFrameStartInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)> function)
{
	this->_onInitFS = function;
}

void K3D12::RenderingPathBase::SetEnable(bool enable)
{
	this->_isEnable = enable;
}

void K3D12::RenderingPathBase::SetCommandList(std::weak_ptr<GraphicsCommandList> list)
{
	this->_commandList = list;
}

bool K3D12::RenderingPathBase::IsEnable()
{
	return _isEnable;
}

