#include "D3D12System.h"

#include "../Util/D3D12Common.h"
#include "../Window/Window.h"
#include "../Device/D3D12Device.h"
#include "../Factory/Factory.h"
#include "../AsyncComponent/Fence.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../CommandContext/CommandQueue.h"
#include "../SwapChain/SwapChain.h"
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
#include "../Audio/AudioManager.h"
#include "../Particle/GPUParticle.h"

#include <random>


using namespace K3D12;

//constexpr unsigned int LIGHT_NUM = 25;
//std::random_device randDev;
//std::mt19937 dev(randDev());
//std::uniform_real_distribution<float> lightRangeZ(-100.0f, 100.0f);
//std::uniform_real_distribution<float> lightRangeX(-100.0f, 100.0f);
//std::uniform_real_distribution<float> lightRangeY(5.0f, 30.0f);
//std::uniform_real_distribution<float> lightRange(30.0f, 80.0f);
//
//std::uniform_real_distribution<float> lightColorR(0.0f, 1.0f);
//std::uniform_real_distribution<float> lightColorG(0.0f, 1.0f);
//std::uniform_real_distribution<float> lightColorB(0.0f, 1.0f);
//
//std::uniform_real_distribution<float> lightAttenuent(1.0f, 50.0f);
//std::uniform_real_distribution<float> lightIntensity(1.0f, 10.0f);



D3D12System::D3D12System() :
	_windowHeight(1280), _windowWidth(720), _appClassName(L"D3D12MiniLib")
{

}


D3D12System::~D3D12System()
{
	TermWindow();
	TermD3D12();
}

D3D12System & D3D12System::GetInstance()
{
	return *_instance;
}


void D3D12System::InitializeCamera(CameraMode mode, const Vector3 & pos, const Vector3 & target, const Vector3 & up, float nearClip, float farClip)
{

	switch (mode)
	{
	case CameraMode::Perspective:
		GetInstance()._mainCamera.InitializeCamera(CameraMode::Perspective, static_cast<float>(GetInstance()._windowWidth), static_cast<float>(GetInstance()._windowHeight), nearClip, farClip, pos, target, up);

		break;
	case CameraMode::Orthogonal:
		GetInstance()._mainCamera.InitializeCamera(CameraMode::Orthogonal, static_cast<float>(GetInstance()._windowWidth), static_cast<float>(GetInstance()._windowHeight), nearClip, farClip, pos, target, up);

		break;
	default:
		break;
	}
	GetInstance()._mainCamera.Update();
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

HRESULT D3D12System::CreateCommandList(std::string name, unsigned int nodeMask,const D3D12_COMMAND_LIST_TYPE & type)
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

std::shared_ptr<GraphicsCommandList> K3D12::D3D12System::GetComputeCommandList()
{
	return GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("ComputeList");
}

std::shared_ptr<GraphicsCommandList> K3D12::D3D12System::GetCopyCommandList()
{
	return GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CopyList");
}

K3D12::CommandQueue& K3D12::D3D12System::GetMasterCommandQueue()
{
	return GetInstance()._commandQueue;
}

CommandQueue & K3D12::D3D12System::GetMasterComputeQueue()
{
	return GetInstance()._computeQueue;
}

CommandQueue & K3D12::D3D12System::GetMasterCopyQueue()
{
	return GetInstance()._copyQueue;
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

HRESULT K3D12::D3D12System::InitializeComputeQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;

	auto hr = this->_computeQueue.Create(desc);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D12::D3D12System::InitializeCopyQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY;

	auto hr = this->_copyQueue.Create(desc);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT D3D12System::InitializeCommandList()
{
	//マスターのコマンドリスト
	{
		auto hr = GraphicsContextLibrary::GetInstance().CreateCommandList("CommandList", 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CHECK_RESULT(hr);
	}
	//マスターのコンピュートリスト
	{
		auto hr = GraphicsContextLibrary::GetInstance().CreateCommandList("ComputeList", 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
		CHECK_RESULT(hr);
	}
	//マスターのコピーリスト
	{
		auto hr = GraphicsContextLibrary::GetInstance().CreateCommandList("CopyList", 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY);
		CHECK_RESULT(hr);
	}
	return S_OK;
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
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif
	K3D12::HLSLIncluder includes("./Engine/Shader/");

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/SpriteDefaultShader.hlsl", nullptr, &includes, "VsMain", "vs_5_0", compileFlag, 0, &vs, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/SpriteDefaultShader.hlsl", nullptr, &includes, "PsMain", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	,  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザステートの設定
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

	//レンダーターゲットのブレンド設定
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
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	//αが一未満をクリップ
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//パイプラインステートオブジェクト
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	//ここで_counfofが赤波線（エラー）を引き起こしているのはインテリセンスの既知のバグとしてWindowsに報告
	//ビルドは通る
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

	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = FALSE;								//深度テストあり

	//パイプラインステートオブジェクトの生成
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
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	HLSLIncluder includes("./Engine/Shader/");
	HRESULT hr;
	if (FAILED(hr = D3DCompileFromFile(L"./Engine/Shader/PrimitiveShader.hlsl", nullptr, &includes, "VsMain", "vs_5_0", compileFlag, 0, &vs, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		HRESULT_LOG(hr);

		return hr;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/PrimitiveShader.hlsl", nullptr, &includes, "PsMain", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		HRESULT_LOG(hr);
		return hr;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザステートの設定
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

	//レンダーターゲットのブレンド設定
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
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//パイプラインステートオブジェクト
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

	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//深度テストあり
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//ステンシルテストなし
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
	//パイプラインステートオブジェクトの生成
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
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
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

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザステートの設定
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

	//レンダーターゲットのブレンド設定
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
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = TRUE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}

	//パイプラインステートオブジェクト
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

	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = FALSE;								//深度テストあり
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//ステンシルテストなし
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
	//パイプラインステートオブジェクトの生成
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
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
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

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザステートの設定
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

	//レンダーターゲットのブレンド設定
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
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//パイプラインステートオブジェクト
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
	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//深度テストあり
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//ステンシルテストなし
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
	//パイプラインステートオブジェクトの生成
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
	this->_mainCamera.InitializeCamera(CameraMode::Perspective, static_cast<float>(this->_windowWidth), static_cast<float>(this->_windowHeight), 1.0f, 500.0f, cameraPos, target, upward);
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

HRESULT K3D12::D3D12System::InitializeGPUParticle()
{
	return S_OK;
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
	hr = InitializeComputeQueue();
	CHECK_RESULT(hr);
	hr = InitializeCopyQueue();
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
	DEBUG_LOG(std::string("Terminate Window"));

}

void D3D12System::TermD3D12()
{

	_modelPool.Discard();
	GraphicsContextLibrary::GetInstance().Discard();
	_commandQueue.Discard();
	_copyQueue.Discard();
	_computeQueue.Discard();
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
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_FLAGS::D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS::D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}

	DEBUG_LOG(std::string("Terminate D3D12"));

	AudioManager::GetInstance().Discard();
	DEBUG_LOG(std::string("Terminate XAudio2"));
	CoUninitialize();
	DEBUG_LOG(std::string("COMの終了"));

}

void K3D12::Create()
{
	if (D3D12System::GetInstance()._instance == nullptr) {

		D3D12System::GetInstance()._instance = new D3D12System();
		DEBUG_LOG(std::string("Create K3DSystem"));
	}
	return;
}

void K3D12::Destroy()
{
	if (D3D12System::GetInstance()._instance != nullptr) {

		delete D3D12System::GetInstance()._instance;
		DEBUG_LOG(std::string("Delete K3DSystem"));
	}
	return;
}

HRESULT K3D12::Initialize(UINT windowWidth, UINT windowHeight, UINT backBufferCount, bool useWarpDevice)
{
	SystemLogger::GetInstance().SetFilter(LogLevel::Details);
	K3D12::SetWindowSize(windowWidth, windowHeight);
	auto hr = D3D12System::GetInstance().InitializeWindow();
	CHECK_RESULT(hr);
	DEBUG_LOG(std::string("ウインドウが正常に初期化されました"));
	CHECK_RESULT(CoInitializeEx(NULL, COINIT_MULTITHREADED));
	DEBUG_LOG(std::string("COMが初期化されました"));
	hr = D3D12System::GetInstance().InitializeD3D12(backBufferCount, useWarpDevice);
	CHECK_RESULT(hr);
	DEBUG_LOG(std::string("D3D12が正常に初期化されました"));
	K3D12::D3D12System::InitializeController();
	DEBUG_LOG(std::string("コントローラの最大接続数を４で初期化しました"));
	K3D12::D3D12System::GetInstance()._inputManager.SetFocusWindow(K3D12::D3D12System::GetInstance().GetWindow().GetWindowHandle());
	return S_OK;
}

HRESULT K3D12::Initialize(UINT backBufferCount, bool useWarpDevice)
{
	SystemLogger::GetInstance().SetFilter(LogLevel::Details);
	auto hr = D3D12System::GetInstance().InitializeWindow();
	CHECK_RESULT(hr);
	DEBUG_LOG(std::string("ウインドウが正常に初期化されました"));
	hr = D3D12System::GetInstance().InitializeD3D12(backBufferCount, useWarpDevice);
	CHECK_RESULT(hr);
	DEBUG_LOG(std::string("D3D12が正常に初期化されました"));
	K3D12::D3D12System::InitializeController();
	DEBUG_LOG(std::string("コントローラの最大接続数を４で初期化しました"));
	K3D12::D3D12System::GetInstance()._inputManager.SetFocusWindow(K3D12::D3D12System::GetInstance().GetWindow().GetWindowHandle());

	return S_OK;
}

void K3D12::ScreenFlip()
{
	//最終レンダリングコミット
	{
		//レンダーターゲットをプレセント状態へ
		D3D12System::GetInstance()._renderTarget.SetStatePresent(D3D12System::GetInstance().GetCommandList("CommandList"));
		//コマンドリストを閉じる
		D3D12System::GetInstance().GetCommandList("CommandList")->CloseCommandList();
		//コマンド実行
		{
			ID3D12CommandList *lists[] = { D3D12System::GetInstance().GetCommandList("CommandList")->GetCommandList().Get() };
			D3D12System::GetInstance()._commandQueue.GetQueue()->ExecuteCommandLists(_countof(lists), lists);

		}
		//プレセント
		D3D12System::GetInstance()._renderTarget.Present(1,0);
	}
	//GPU待機
	D3D12System::GetInstance()._fence.Wait(&D3D12System::GetInstance()._commandQueue);
	//リストとアロケータをリセットしてレコード状態へ
	D3D12System::GetInstance().GetCommandList("CommandList")->ResetAllocator();
	D3D12System::GetInstance().GetCommandList("CommandList")->ResetCommandList();
}

void K3D12::ClearScreen()
{
	//レンダーターゲットをプレセント状態へ
	//クリア処理をコマンドリストに積む
	auto& ref = D3D12System::GetInstance();
	ref._renderTarget.ClearScreen(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList("CommandList"));
	ref.GetCommandList("CommandList")->GetCommandList()->RSSetScissorRects(1, &ref._window.GetScissorRect());
	ref.GetCommandList("CommandList")->GetCommandList()->RSSetViewports(1, &ref._window.GetViewPort());
	ref._mainCamera.GetDepthStencil().ClearDepthStencil(ref.GetCommandList("CommandList"));

}

Camera & K3D12::GetCamera()
{
	return D3D12System::GetInstance()._mainCamera;
}

ID3D12Device * K3D12::GetDevice()
{
	return D3D12System::GetInstance()._device.GetDevice().Get();
}

IDXGIFactory4 * K3D12::GetFactory()
{
	return D3D12System::GetInstance()._factory.GetFactory().Get();
}

Fence * K3D12::GetFence()
{
	return &D3D12System::GetInstance()._fence;
}

InputManager & K3D12::Input()
{
	return (D3D12System::GetInstance()._inputManager);
}

void K3D12::SetWindowSize(UINT widths, UINT height)
{
	K3D12::Create();
	D3D12System::GetInstance()._windowHeight = height;
	D3D12System::GetInstance()._windowWidth = widths;
}

void K3D12::SetWindowName(std::wstring name)
{
	K3D12::Create();
	D3D12System::GetInstance()._appClassName = name;
}

void K3D12::SetMainRenderTarget(std::weak_ptr<GraphicsCommandList> list,D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle)
{
	D3D12System::GetInstance()._renderTarget.SetRenderTarget(list.lock(), depthHandle);
}

int K3D12::MessageLoop()
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

std::weak_ptr<MMDModel> K3D12::LoadModel(std::string modelPath)
{
	return D3D12System::GetInstance()._modelPool.LoadModel(modelPath);
}
