#include <string>
#include "Factory.h"
#include "../Util/Logger.h"


K3D12::Factory::Factory() :
	_factory()
{
}


K3D12::Factory::~Factory()
{
	_factory.Reset();
}


HRESULT K3D12::Factory::Create(UINT createFlags)
{
	UINT flag = 0;
#if defined(_DEBUG)
	// DirectX12�̃f�o�b�O���C���[��L���ɂ���
	{
		Microsoft::WRL::ComPtr<ID3D12Debug>	debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

		}
	}
	createFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DirectX12���T�|�[�g���闘�p�\�ȃn�[�h�E�F�A�A�_�v�^���擾
	if (FAILED(CreateDXGIFactory2(createFlags, IID_PPV_ARGS(&_factory))))
		return FALSE;
	return S_OK;
}

Microsoft::WRL::ComPtr<IDXGIFactory6> K3D12::Factory::GetFactory()const
{
	return _factory;
}

void K3D12::Factory::Discard()
{
	if (_factory != nullptr) {
		_factory.Reset();
		DEBUG_LOG(std::string("Factory is Reset"));
	}
}

