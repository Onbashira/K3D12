#include "D3D12System.h"

#include "../Util/D3D12Common.h"
#include "../Window/Window.h"
#include "../Device/D3D12Device.h"
#include "../Factory/Factory.h"
#include "../AsyncComponent/Fence.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../CommandContext/CommandQueue.h"
#include "../Resource/RenderTarget.h"
#include "../Input/InputManager.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Model/ModelPool.h"
#include "../Input/GamePadManager.h"
#include "../Rendering/GBufferSprite.h"
#include "../GameConponent/Primitive/PrimitiveCreater.h"
#include "../Util/Logger.h"
#include "../ShaderComponent/HLSLIncluder.h"
#include "../TextureComponent/TextureManager.h"
#include "../GameConponent/Light/LightManager.h"
#include "../GameConponent/Primitive/PrimitiveCreater.h"
#include "../Signature/RootSignature.h"

#include <random>


using namespace K3D12;

constexpr unsigned int LIGHT_NUM = 25;
std::random_device randDev;
std::mt19937 dev(randDev());
std::uniform_real_distribution<float> lightRangeZ(-100.0f, 100.0f);
std::uniform_real_distribution<float> lightRangeX(-100.0f, 100.0f);
std::uniform_real_distribution<float> lightRangeY(5.0f, 30.0f);
std::uniform_real_distribution<float> lightRange(30.0f, 80.0f);

std::uniform_real_distribution<float> lightColorR(0.0f, 1.0f);
std::uniform_real_distribution<float> lightColorG(0.0f, 1.0f);
std::uniform_real_distribution<float> lightColorB(0.0f, 1.0f);

std::uniform_real_distribution<float> lightAttenuent(1.0f, 50.0f);
std::uniform_real_distribution<float> lightIntensity(1.0f, 10.0f);



D3D12System::D3D12System() :
	_windowHeight(1280), _windowWidth(720), _appClassName(L"D3D12MiniLib")
{

}


D3D12System::~D3D12System()
{
	TermWindow();
	TermD3D12();
}


IDXGIFactory4* D3D12System::GetFactory()
{
	return _factory.GetFactory();
}

K3D12::Fence * D3D12System::GetFence()
{
	return &_fence;
}

InputManager& D3D12System::GetInput()
{
	return (D3D12System::GetInstance()._inputManager);
}

K3D12::CommandQueue * D3D12System::GetCommandQueue()
{
	return &_commandQueue;
}

void D3D12System::Create()
{
	if (_instance == nullptr) {
		_instance = new D3D12System();
	}
	return;
}

D3D12System & D3D12System::GetInstance()
{
	return *_instance;
}

void D3D12System::Destroy()
{
	if (_instance != nullptr) {
		delete _instance;
	}
	return;
}

HRESULT D3D12System::Initialize(UINT windowWidth, UINT windowHeight, UINT backBufferCount, bool useWarpDevice)
{
	SystemLogger::GetInstance().SetFilter(LogLevel::Details);
	D3D12System::GetInstance().SetWindowSize(windowWidth, windowHeight);
	auto hr = D3D12System::GetInstance().InitializeWindow();
	CHECK_RESULT(hr);
	SystemLogger::GetInstance().Log(LogLevel::Info, "�E�C���h�E������ɏ���������܂���\n");
	hr = D3D12System::GetInstance().InitializeD3D12(backBufferCount, useWarpDevice);
	CHECK_RESULT(hr);
	SystemLogger::GetInstance().Log(LogLevel::Info, "D3D12������ɏ���������܂���\n");
	return S_OK;
}

HRESULT D3D12System::Initialize(UINT backBufferCount, bool useWarpDevice)
{
	SystemLogger::GetInstance().SetFilter(LogLevel::Details);
	auto hr = D3D12System::GetInstance().InitializeWindow();
	CHECK_RESULT(hr);
	SystemLogger::GetInstance().Log(LogLevel::Info, "�E�C���h�E������ɏ���������܂���\n");
	hr = D3D12System::GetInstance().InitializeD3D12(backBufferCount, useWarpDevice);
	CHECK_RESULT(hr);
	SystemLogger::GetInstance().Log(LogLevel::Info, "D3D12������ɏ���������܂���\n");

#pragma region Multi Light Test Code
	LightInfo lightInfo;

	//LightManager::Create(LIGHT_NUM);
	//for (unsigned int i = 0; i < LIGHT_NUM; ++i) {
	//	lightInfo.range = lightRange(dev);
	//	lightInfo.attenuation = lightAttenuent(dev);
	//	lightInfo.color = Vector3(lightColorR(dev), lightColorG(dev), lightColorB(dev));
	//	lightInfo.type = 0;
	//	lightInfo.intensity = 1.0f;
	//	lightInfo.pos = Vector3(lightRangeX(dev), lightRangeY(dev), lightRangeZ(dev));

	//	LightManager::AddLight(lightInfo);
	//}
	LightManager::Create(1);
	lightInfo.range = 1000.0f;
	lightInfo.attenuation = 100.0f;
	lightInfo.color = Vector3::one;
	lightInfo.type = 1;
	lightInfo.intensity = 1.0f;
	lightInfo.direction = Vector3(1.0f, -1.0f, 1.0f).Normalize();
	lightInfo.pos = Vector3(-200.0f, 200.0f, -200.0f);
	LightManager::AddLight(lightInfo);

	LightManager::CommitStructuredBuffer();
#pragma endregion
	return S_OK;
}

void D3D12System::InitializeCamera(CameraType type, const Vector3 & pos, const Vector3 & target, const Vector3 & up, float nearClip, float farClip, float fov)
{

	switch (type)
	{
	case CameraType::Perspective:
		GetInstance()._mainCamera.InitializeCamera(CameraType::Perspective, static_cast<float>(GetInstance()._windowWidth), static_cast<float>(GetInstance()._windowHeight), 1.0f, 500.0f, pos, target, up);

		break;
	case CameraType::Orthogonal:
		GetInstance()._mainCamera.InitializeCamera(CameraType::Orthogonal, static_cast<float>(GetInstance()._windowWidth), static_cast<float>(GetInstance()._windowHeight), 1.0f, 500.0f, pos, target, up);

		break;
	default:
		break;
	}
	GetInstance()._mainCamera.Update();
}

void D3D12System::SetWindowSize(UINT widths, UINT height)
{
	D3D12System::Create();
	D3D12System::GetInstance()._windowHeight = height;
	D3D12System::GetInstance()._windowWidth = widths;
}

void D3D12System::SetWindowName(std::wstring name)
{
	D3D12System::Create();
	D3D12System::GetInstance()._appClassName = name;
}

int D3D12System::MessageLoop()
{
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return -1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return 0;
		}
	}
}

void D3D12System::ScreenFlip()
{
	//��Œǉ��\�Ȑ݌v�ɕύX����
	auto& ref = D3D12System::GetInstance();
	//�W�I���g���`��p�X
	//�R�}���h���s
	{
		auto cameraInfo = ref._mainCamera.GetCameraInfo();
		ref._geometryBufferSprite.SetCameraInfo(cameraInfo);
		GraphicsContextLibrary::GetInstance().CloseAllCommandLists();
		GraphicsContextLibrary::GetInstance().ExcutionAllCommandLists(&D3D12System::GetInstance()._commandQueue, &D3D12System::GetInstance()._fence);
		GraphicsContextLibrary::GetInstance().ResetAllCommandList();
	}
	D3D12System::GetInstance()._geometryBufferSprite.TransitionGeometryPassEnd();
	//���C�e�B���O�p�X
	//�R�}���h���s(���̎��_�Ńe�N�X�`���������s��)
	{
		D3D12System::GetInstance()._geometryBufferSprite.TransitionLightingPassStart();
		ref._geometryBufferSprite.StartLightingPass(LightManager::GetLightUAV(), &ref._mainCamera.GetDepthStencil());
		GraphicsContextLibrary::GetInstance().CloseCommandList("CommandList");
		ID3D12CommandList *lists[] = { ref.GetCommandList("CommandList")->GetCommandList().Get() };
		D3D12System::GetInstance()._commandQueue.GetQueue()->ExecuteCommandLists(_countof(lists), lists);
		D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
		GraphicsContextLibrary::GetInstance().ResetCommandList("CommandList");
	}
	D3D12System::GetInstance()._geometryBufferSprite.TransitionLightingPassEnd();
	//�������I�u�W�F�N�g�`��p�X
	{

	}
	//���������C�e�B���O�p�X
	{

	}
	//�A���`�G�C���A�X���̃|�X�g�G�t�F�N�g�p�X
	//�R�}���h���s
	{
		//�߂�ǂ��Ȃ̂ł����ōĒ�`
		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
		//
		{
			//���ʃe�N�X�`���ƒ��_�l�𓊂���
			ref._geometryBufferSprite.TransitionResultTexture2ToRT();
			ref._geometryBufferSprite.TransitionResultTextureToSR();
			ref.GetCommandList("CommandList")->GetCommandList()->ClearRenderTargetView(ref._geometryBufferSprite.GetRTVCPUHandle(GEOMETRY_TYPE::RESULT2), clearValue.Color, 0, nullptr);

			ref.GetCommandList("CommandList")->OMSetRenderTargets(1, &ref._geometryBufferSprite.GetRTVCPUHandle(GEOMETRY_TYPE::RESULT2), FALSE);
			ref.GetCommandList("CommandList")->GetCommandList()->IASetVertexBuffers(0, 1, &ref._geometryBufferSprite.GetVertexBuffer().GetView());
			ref.GetCommandList("CommandList")->GetCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			ref.GetCommandList("CommandList")->GetCommandList()->SetPipelineState(ref.GetPSO("FxAA")->GetPSO().Get());
			ref.GetCommandList("CommandList")->GetCommandList()->SetGraphicsRootSignature(ref.GetPSO("FxAA")->GetRootSignature().lock()->GetSignature().Get());
			ref.GetCommandList("CommandList")->GetCommandList()->RSSetScissorRects(1, &ref._window.GetScissorRect());
			ref.GetCommandList("CommandList")->GetCommandList()->RSSetViewports(1, &ref._window.GetViewPort());
			ref._geometryBufferSprite.SetHeaps();
			ref.GetCommandList("CommandList")->GetCommandList()->SetGraphicsRootDescriptorTable(0, ref._geometryBufferSprite.GetSRVGPUHandle(GEOMETRY_TYPE::RESULT));
			ref.GetCommandList("CommandList")->GetCommandList()->DrawInstanced(4, 1, 0, 0);

			ref._geometryBufferSprite.TransitionResultTextureToGeneric();
			ref._geometryBufferSprite.TransitionResultTexture2ToGeneric();

		}
		GraphicsContextLibrary::GetInstance().CloseCommandList("CommandList");
		ID3D12CommandList *lists[] = { ref.GetCommandList("CommandList")->GetCommandList().Get() };
		D3D12System::GetInstance()._commandQueue.GetQueue()->ExecuteCommandLists(_countof(lists), lists);
		D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
		GraphicsContextLibrary::GetInstance().ResetCommandList("CommandList");
	}
	//�f�B�t�@�[�h�̃��U���g�e�N�X�`�����R�s�[����
	{
		ref._renderTarget.SetStateCopyDest(ref.GetCommandList("CommandList"));
		ref._geometryBufferSprite.TransitionResultTexture2ToCopySource();
		ref._renderTarget.CopyToRenderTarget(ref.GetCommandList("CommandList"), ref._geometryBufferSprite.GetGBufferResource(GEOMETRY_TYPE::RESULT2));
		ref._geometryBufferSprite.TransitionResultTexture2ToGeneric();
		ref._renderTarget.SetStateGenericRead(ref.GetCommandList("CommandList"));
		GraphicsContextLibrary::GetInstance().CloseCommandList("CommandList");
		ID3D12CommandList *lists[] = { ref.GetCommandList("CommandList")->GetCommandList().Get() };
		D3D12System::GetInstance()._commandQueue.GetQueue()->ExecuteCommandLists(_countof(lists), lists);
		D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
		GraphicsContextLibrary::GetInstance().ResetCommandList("CommandList");


	}
	//�ŏI�����_�����O�R�~�b�g
	{
		//�����_�[�^�[�Q�b�g���v���Z���g��Ԃ�
		D3D12System::GetInstance()._renderTarget.SetStatePresent(D3D12System::GetInstance().GetCommandList("CommandList"));
		//�R�}���h���X�g�����
		D3D12System::GetInstance().GetCommandList("CommandList")->CloseCommandList();
		//�R�}���h���s
		{
			ID3D12CommandList *lists[] = { ref.GetCommandList("CommandList")->GetCommandList().Get() };
			D3D12System::GetInstance()._commandQueue.GetQueue()->ExecuteCommandLists(_countof(lists), lists);
			D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
		}
		//�v���Z���g
		D3D12System::GetInstance()._renderTarget.Present(1, 0);
	}
	//GPU�ҋ@
	D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
	//���X�g�ƃA���P�[�^�����Z�b�g���ă��R�[�h��Ԃ�
	D3D12System::GetInstance().GetCommandList("CommandList")->ResetAllocator();
	D3D12System::GetInstance().GetCommandList("CommandList")->ResetCommandList();

	//�`��R�}���h���l�܂��I�u�W�F�N�g�Ƀf�v�X�X�e���V���r���[�Ƀf�t�H���g�̃r���[�|�[�g�Ɛ؂����`�A�����DSVRTV���Z�b�g

}

void D3D12System::ClearScreen()
{
	//�N���A�������R�}���h���X�g�ɐς�
	auto& ref = D3D12System::GetInstance();
	ref._renderTarget.ClearScreen(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"));
	ref.GetCommandList("CommandList")->GetCommandList()->RSSetScissorRects(1, &ref._window.GetScissorRect());
	ref.GetCommandList("CommandList")->GetCommandList()->RSSetViewports(1, &ref._window.GetViewPort());
	ref._mainCamera.GetDepthStencil().ClearDepthStencil(ref.GetCommandList("CommandList"));
	D3D12System::GetInstance()._geometryBufferSprite.TransitionGeometryPassStart();
	//�h���[�R�[�����Ă΂��\���̂���s���������_�����O���T�|�[�g����R�}���h���X�g�ɑ΂��ăf�B�t�@�[�h�����_�����O�ŗp����e�N�X�`�����o�C���h����B
	ref.BindingGBufferRenderTarget();

}

HRESULT D3D12System::LoadTexture(std::string path)
{

	return E_NOTIMPL;
}

std::weak_ptr<MMDModel> D3D12System::LoadModel(std::string modelPath)
{
	return GetInstance()._modelPool.LoadModel(modelPath);
}

std::shared_ptr<GamePad> D3D12System::GetController(int padID)
{
	return D3D12System::GetInstance()._inputManager.GetGamePad(padID);
}

void D3D12System::InitializeController(int controllerCount)
{
	D3D12System::GetInstance()._inputManager.GamePadInitialize(controllerCount);
}

K3D12::Window & D3D12System::GetWindow()
{
	return D3D12System::GetInstance()._window;
}

void D3D12System::InputUpdate()
{
	D3D12System::GetInstance()._inputManager.InputUpdate();
}

HRESULT D3D12System::CreatePSO(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & desc, ID3DBlob* rootSignature)
{

	return GraphicsContextLibrary::GetInstance().CreatePSO(name, desc, rootSignature);;
}

HRESULT D3D12System::CreatePSO(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & desc, ID3DBlob* rootSignature)
{
	return GraphicsContextLibrary::GetInstance().CreatePSO(name, desc, rootSignature);;
}

HRESULT D3D12System::CreateCommandList(std::string name, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE & type)
{
	return GraphicsContextLibrary::GetInstance().CreateCommandList(name, nodeMask, type);
}

std::shared_ptr<K3D12::PipelineStateObject> D3D12System::GetPSO(std::string name)
{
	return GraphicsContextLibrary::GetInstance().GetPSO(name);
}

std::shared_ptr<K3D12::GraphicsCommandList> D3D12System::GetCommandList(std::string name)
{
	return GraphicsContextLibrary::GetInstance().GetGraphicsCommandList(name);
}

std::shared_ptr<K3D12::GraphicsCommandList> K3D12::D3D12System::GetMasterCommandList()
{
	return GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList");
}

K3D12::CommandQueue& K3D12::D3D12System::GetMasterCommandQueue()
{
	return GetInstance()._commandQueue;
}

Cube * K3D12::D3D12System::CreateCube()
{
	return _primitiveCreater.CreateCube();
}

Camera & D3D12System::GetCamera()
{
	return GetInstance()._mainCamera;
}

ID3D12Device * D3D12System::GetDevice()
{
	return _device.GetDevice();
}

HRESULT D3D12System::InitializeDevice(bool useWarpDevice)
{
	auto hr = _device.Create(&_factory, useWarpDevice);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeDXGIFactory()
{
	auto hr = _factory.Create();
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeRenderTargets(unsigned int bufferCount)
{
	auto hr = _renderTarget.Create(_commandQueue, _factory, _window, _windowWidth, _windowHeight, bufferCount);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;

	auto hr = this->_commandQueue.Create(desc);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeCommandList()
{
	auto hr = GraphicsContextLibrary::GetInstance().CreateCommandList("CommandList", 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeFence()
{
	auto hr = _fence.Create(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializePSO()
{
	auto hr = InitializeSpritePSO();
	CHECK_RESULT(hr);
	hr = InitializeModelOpacity();
	CHECK_RESULT(hr);
	hr = InitializePrimitivePSO();
	CHECK_RESULT(hr);
	hr = InitializeFxAAPSO();
	CHECK_RESULT(hr);

	return S_OK;
}

HRESULT D3D12System::InitializeSpritePSO()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vs;
	Microsoft::WRL::ComPtr<ID3DBlob> gs;
	Microsoft::WRL::ComPtr<ID3DBlob> ds;
	Microsoft::WRL::ComPtr<ID3DBlob> hs;
	Microsoft::WRL::ComPtr<ID3DBlob> ps;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

#if defined(_DEBUG)
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif
	K3D12::HLSLIncluder includes("./Engine/Shader/");

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/SpriteShader.hlsl", nullptr, &includes, "VsMain", "vs_5_0", compileFlag, 0, &vs, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/SpriteShader.hlsl", nullptr, &includes, "PsMain", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//���_���̓��C�A�E�g�̒�`
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = TRUE;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	descRTBS.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	descRTBS.BlendOp = D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	descRTBS.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	descRTBS.LogicOp = D3D12_LOGIC_OP_CLEAR;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	//�����ꖢ�����N���b�v
	descBS.AlphaToCoverageEnable = TRUE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	//������_counfof���Ԕg���i�G���[�j�������N�����Ă���̂̓C���e���Z���X�̊��m�̃o�O�Ƃ���Windows�ɕ�
	//�r���h�͒ʂ�
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = vs->GetBufferPointer();
		shaderBytecode.BytecodeLength = vs->GetBufferSize();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = ps->GetBufferPointer();
		shaderBytecode.BytecodeLength = ps->GetBufferSize();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	psoDesc.DepthStencilState.DepthEnable = FALSE;								//�[�x�e�X�g����

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	auto hr = GraphicsContextLibrary::GetInstance().CreatePSO("SpritePSO", psoDesc, vs.Get());
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializePrimitivePSO()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vs;
	Microsoft::WRL::ComPtr<ID3DBlob> gs;
	Microsoft::WRL::ComPtr<ID3DBlob> ds;
	Microsoft::WRL::ComPtr<ID3DBlob> hs;
	Microsoft::WRL::ComPtr<ID3DBlob> ps;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

#if defined(_DEBUG)
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	HLSLIncluder includes("./Engine/Shader/");
	HRESULT hr;
	if (FAILED(hr = D3DCompileFromFile(L"./Engine/Shader/PrimitiveShader.hlsl", nullptr, &includes, "VsMain", "vs_5_0", compileFlag, 0, &vs, &error))) {
		SystemLogger::GetInstance().Log(LogLevel::Warning, hr);
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/PrimitiveShader.hlsl", nullptr, &includes, "PsMain", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//���_���̓��C�A�E�g�̒�`
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = FALSE;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	descRTBS.DestBlend = D3D12_BLEND_DEST_COLOR;
	descRTBS.BlendOp = D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	descRTBS.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
	descRTBS.LogicOp = D3D12_LOGIC_OP_CLEAR;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = vs->GetBufferPointer();
		shaderBytecode.BytecodeLength = vs->GetBufferSize();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = ps->GetBufferPointer();
		shaderBytecode.BytecodeLength = ps->GetBufferSize();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//�[�x�e�X�g����
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//�X�e���V���e�X�g�Ȃ�
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	hr = GraphicsContextLibrary::GetInstance().CreatePSO("Primitive", psoDesc, vs.Get());
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeFxAAPSO()
{

	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> error = {};
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;
#if defined(_DEBUG)
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif
	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/QualityFXAA.hlsl", nullptr, nullptr, "FxaaVSMain", "vs_5_0", compileFlag, 0, &vertexShader, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}
	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/QualityFXAA.hlsl", nullptr, nullptr, "FxaaPSMain", "ps_5_0", compileFlag, 0, &pixelShader, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//���_���̓��C�A�E�g�̒�`
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND_ONE;
	descRTBS.DestBlend = D3D12_BLEND_ZERO;
	descRTBS.BlendOp = D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND_ONE;
	descRTBS.DestBlendAlpha = D3D12_BLEND_ZERO;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.LogicOp = D3D12_LOGIC_OP_CLEAR;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = TRUE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	psoDesc.pRootSignature = nullptr;
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
		shaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
		shaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	psoDesc.DepthStencilState.DepthEnable = FALSE;								//�[�x�e�X�g����
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//�X�e���V���e�X�g�Ȃ�
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	auto hr = GraphicsContextLibrary::GetInstance().CreatePSO("FxAA", psoDesc, vertexShader.Get());

	vertexShader.Reset();
	error.Reset();
	pixelShader.Reset();
	return hr;
}

HRESULT D3D12System::InitializeModelTransparency()
{
	return E_NOTIMPL;
}

HRESULT D3D12System::InitializeModelOpacity()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vs;
	Microsoft::WRL::ComPtr<ID3DBlob> gs;
	Microsoft::WRL::ComPtr<ID3DBlob> ds;
	Microsoft::WRL::ComPtr<ID3DBlob> hs;
	Microsoft::WRL::ComPtr<ID3DBlob> ps;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

#if defined(_DEBUG)
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	HLSLIncluder includes("./Engine/Shader/");

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/MMDModelOpacity.hlsl", nullptr, &includes, "VsMain", "vs_5_0", compileFlag, 0, &vs, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/MMDModelOpacity.hlsl", nullptr, &includes, "PsMain", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//���_���̓��C�A�E�g�̒�`
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = TRUE;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;
	descRTBS.DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;
	descRTBS.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	descRTBS.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	descRTBS.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_CLEAR;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = vs->GetBufferPointer();
		shaderBytecode.BytecodeLength = vs->GetBufferSize();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = ps->GetBufferPointer();
		shaderBytecode.BytecodeLength = ps->GetBufferSize();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//�[�x�e�X�g����
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//�X�e���V���e�X�g�Ȃ�
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	auto hr = GraphicsContextLibrary::GetInstance().CreatePSO("MMDModelOpacity", psoDesc, vs.Get());
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeModelPool()
{

	_modelPool.SetGraphicsCommandList(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"));
	_modelPool.SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("MMDModelOpacity"));
	_modelPool.SetRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("MMDModelOpacity"));
	return S_OK;
}

HRESULT D3D12System::InitializePrimitiveCreater()
{
	_primitiveCreater.SetPrimitiveDrawCommandList(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"));
	_primitiveCreater.SetPrimitivePipelineState(GraphicsContextLibrary::GetInstance().GetPSO("Primitive"));
	_primitiveCreater.SetPrimitiveRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("Primitive"));
	return S_OK;
}

HRESULT D3D12System::InitializeMainCamera()
{
	Vector3 cameraPos(0.0f, 20.0f, -20.0f);
	Vector3 target = Vector3(0, 10.0f, 0);
	Vector3 upward = Vector3::up;
	this->_mainCamera.InitializeCamera(CameraType::Perspective, static_cast<float>(this->_windowWidth), static_cast<float>(this->_windowHeight), 1.0f, 500.0f, cameraPos, target, upward);
	auto hr = this->_mainCamera.InitializeCameraDepthStencill(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, _windowWidth, _windowHeight);
	CHECK_RESULT(hr);

	_mainCamera.Update();
	return S_OK;
}

HRESULT D3D12System::CommandListConnecting()
{
	auto hr = GraphicsContextLibrary::GetInstance().Connecting("CommandList", "MMDModelOpacity");
	return hr;
}

HRESULT D3D12System::BindingGBufferRenderTarget()
{
	//#ifdef _DIFFERED_RENDERING__
	_geometryBufferSprite.BindingCommandList(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"));
	_geometryBufferSprite.SetGeometryRenderTarget();

	//#endif
	return S_OK;
}

HRESULT D3D12System::InitiazlieGBufferSprite()
{
	HRESULT result{};

	result = _geometryBufferSprite.CreateGBuffer(_windowWidth, _windowHeight, &this->_window, GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"), &_mainCamera.GetDepthStencil());

	return result;
}

HRESULT D3D12System::InitializeWindow()
{
	auto hr = _window.Create(_appClassName, _windowWidth, _windowHeight);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeD3D12(unsigned int bufferCount, bool useWarpDevice)
{
	HRESULT hr;

	hr = InitializeDXGIFactory();
	CHECK_RESULT(hr);
	hr = InitializeDevice(useWarpDevice);
	CHECK_RESULT(hr);
	hr = InitializeFence();
	CHECK_RESULT(hr);
	hr = InitializeCommandQueue();
	CHECK_RESULT(hr);
	hr = InitializeRenderTargets(bufferCount);
	CHECK_RESULT(hr);
	hr = InitializeCommandList();
	CHECK_RESULT(hr);
	hr = InitializePSO();
	CHECK_RESULT(hr);
	hr = InitializeModelPool();
	CHECK_RESULT(hr);
	hr = InitializePrimitiveCreater();
	CHECK_RESULT(hr);
	hr = InitializeMainCamera();
	CHECK_RESULT(hr);
	hr = CommandListConnecting();
	CHECK_RESULT(hr);
	hr = InitiazlieGBufferSprite();
	CHECK_RESULT(hr);
	return S_OK;
}

void D3D12System::TermWindow()
{
	_window.Discard();
}

void D3D12System::TermD3D12()
{

	_modelPool.Discard();
	GraphicsContextLibrary::GetInstance().Discard();
	_commandQueue.Discard();
	_factory.Discard();
	_fence.Discard();
	LightManager::Destroy();
	TextureManager::GetInstance().Discard();
	_geometryBufferSprite.Discard();
	_renderTarget.Discard();
	_mainCamera.Discard();

	ID3D12DebugDevice* debugInterface;
	if (SUCCEEDED(_device.GetDevice()->QueryInterface(&debugInterface)))
	{
		_device.Discard();
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_FLAGS::D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS ::D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}


}