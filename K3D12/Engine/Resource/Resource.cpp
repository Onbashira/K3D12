#include <string>
#include <tuple>
#include "Resource.h"
#include "../Util/Utility.h"
#include "../System/D3D12System.h"
#include "../Util/Logger.h"
#include "../CommandContext/GraphicsCommandList.h"


K3D12::Resource::Resource() :
	_resource(), _pDst(nullptr),
	_clearValue({}), _currentResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON)
	, _name("UnNamedResource ")
{
}

K3D12::Resource::Resource(const Resource & other)
{
	*this = other;
}

K3D12::Resource::Resource(Resource && value)
{
	*this = std::move(value);
}

K3D12::Resource & K3D12::Resource::operator=(const Resource & value)
{
	this->Discard();
	this->_clearValue = value._clearValue;
	this->_currentResourceState = value._currentResourceState;
	this->_name = value._name;
	this->_pDst = value._pDst;
	this->_resource.Attach(value._resource.Get());
	this->_shaderRegisterNumber = value._shaderRegisterNumber;
	return *this;
}

K3D12::Resource & K3D12::Resource::operator=(Resource && value)
{
	*this = value;
	value.Discard();
	value._clearValue = D3D12_CLEAR_VALUE();
	value._currentResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
	value._name = "MOVED RESOURCE";
	value._pDst = nullptr;
	value._resource.Detach();
	value._shaderRegisterNumber = -1;
	return *this;
}


K3D12::Resource::~Resource()
{
	Discard();
}


HRESULT K3D12::Resource::Create(D3D12_HEAP_PROPERTIES heapProps, D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES state, D3D12_CLEAR_VALUE* clearValue)
{

	_currentResourceState = state;
	if (clearValue != nullptr) {
		_clearValue = *clearValue;
	}

	if (_resource.Get() != nullptr) {
		Discard();
	}

	auto hr = GET_DEVICE->CreateCommittedResource(&heapProps, flags, &resourceDesc, state, clearValue, IID_PPV_ARGS(&this->_resource));

	if (FAILED(hr)) {
		SystemLogger::GetInstance().Log(LogLevel::Error, hr);

		return E_FAIL;
	}
	return S_OK;
}

HRESULT K3D12::Resource::Map(UINT subResource, D3D12_RANGE * readRange)
{
	if (_resource.Get() != nullptr) {

		auto hr = _resource->Map(subResource, readRange, reinterpret_cast<void**>(&_pDst));
		if (FAILED(hr)) {
			return E_FAIL;
		}
	}
	return S_OK;

}

void K3D12::Resource::Unmap(UINT subResource, D3D12_RANGE * writtenRange)
{
	if (_resource.Get() != nullptr) {
		_resource->Unmap(subResource, writtenRange);
		_pDst = nullptr;
	}
}

UCHAR* K3D12::Resource::GetMappedPointer()
{
	return _pDst;
}

void K3D12::Resource::Discard()
{
	if (_resource.Get() != nullptr) {
		if (_pDst != nullptr) {
			Unmap(0, nullptr);
		}
		_resource.Reset();
		DEBUG_LOG(std::string("Resource : " + _name + " is  Reset"));
	}
}

void K3D12::Resource::Update(const void * pSrc, ULONG64 size, const UINT dstOffset)
{
	assert(pSrc != nullptr);
	assert(size != 0);
	std::memcpy(_pDst + dstOffset, pSrc, size_t(size));
}

void K3D12::Resource::Read(void * pDstBuffer, ULONG64 readSize, const unsigned int dstOffsest)
{
	assert(pDstBuffer != nullptr);
	assert(readSize != 0);
	std::memcpy(pDstBuffer, _pDst + dstOffsest, size_t(readSize));
}

void K3D12::Resource::Update(const void * pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE * readRange, D3D12_RANGE * writtenRange)
{
	Map(subResource, readRange);
	Update(pSrc, size, dstOffset);
	Unmap(subResource, writtenRange);
}

void K3D12::Resource::Alignment256ByteUpdate(const void * pSrc, unsigned int elementSize, unsigned int elementCount)
{
	UINT offset = 0;
	for (unsigned int i = 0; i < elementCount; i++) {
		Update(static_cast<void*>((char*)(pSrc)+i * elementSize), Util::Alignment256Bytes(elementSize), offset);
		offset += static_cast<unsigned int>(Util::Alignment256Bytes(elementSize));
	}
}

ID3D12Resource * K3D12::Resource::GetResource()
{
	return _resource.Get();
}

ID3D12Resource ** K3D12::Resource::GetAddressOf()
{
	return _resource.GetAddressOf();
}

const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> K3D12::Resource::GetHeapPropaties()
{
	D3D12_HEAP_PROPERTIES props;
	D3D12_HEAP_FLAGS flags;
	_resource->GetHeapProperties(&props, &flags);
	return { props, flags };
}

const D3D12_RESOURCE_DESC* K3D12::Resource::GetResourceDesc()
{
	return &_resource->GetDesc();
}

const D3D12_RESOURCE_STATES & K3D12::Resource::GetResourceState()
{
	return   _currentResourceState;
}

void K3D12::Resource::SetResourceState(D3D12_RESOURCE_STATES state)
{
	_currentResourceState = state;
}

HRESULT K3D12::Resource::ResourceTransition(ID3D12GraphicsCommandList * list, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = this->_resource.Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = _currentResourceState;
	resource_barrier.Transition.StateAfter = nextState;
	_currentResourceState = nextState;
	list->ResourceBarrier(1, &resource_barrier);

	return S_OK;

}

HRESULT K3D12::Resource::ResourceTransition(std::weak_ptr<GraphicsCommandList> list, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = this->_resource.Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = _currentResourceState;
	resource_barrier.Transition.StateAfter = nextState;
	_currentResourceState = nextState;
	list.lock()->GetCommandList()->ResourceBarrier(1, &resource_barrier);

	return S_OK;
}

D3D12_CLEAR_VALUE K3D12::Resource::GetClearValue()
{
	return _clearValue;
}

void K3D12::Resource::SetName(std::string name)
{
	_name = name;
	_resource->SetName(Util::StringToWString(_name).c_str());
}

void K3D12::Resource::RegisterShaderSlot(unsigned int number)
{
	this->_shaderRegisterNumber = number;
}