#include "RenderingPathBase.h"


K3D12::RenderingPathBase::RenderingPathBase()
{
}

K3D12::RenderingPathBase::RenderingPathBase(const RenderingPathBase & other)
{
	*this = other;
}

K3D12::RenderingPathBase::RenderingPathBase(RenderingPathBase && other)
{
	*this = std::move(other);
}

K3D12::RenderingPathBase & K3D12::RenderingPathBase::operator=(const RenderingPathBase & other)
{
	this->_commandList = other._commandList;
	this->_isEnable = other._isEnable;
	this->_onInitAE = other._onInitAE;
	this->_onInitBE = other._onInitBE;
	this->_onInitFS = other._onInitFS;
	return *this;
}

K3D12::RenderingPathBase & K3D12::RenderingPathBase::operator=(RenderingPathBase && other)
{
	this->_commandList = other._commandList;
	this->_isEnable = other._isEnable;
	this->_onInitAE = std::move(other._onInitAE);
	this->_onInitBE = std::move(other._onInitBE);
	this->_onInitFS = std::move(other._onInitFS);

	other._commandList.reset();
	other._isEnable = false;
	return *this;

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

K3D12::RenderingPathBase& K3D12::RenderingPathBase::SetEnable(bool enable)
{
	this->_isEnable = enable;
	return *this;

}

K3D12::RenderingPathBase& K3D12::RenderingPathBase::SetCommandList(std::weak_ptr<GraphicsCommandList> list)
{
	this->_commandList = list;
	return *this;
}

bool K3D12::RenderingPathBase::IsEnable()
{
	return _isEnable;
}

