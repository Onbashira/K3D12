#include <vector>
#include <memory>
#include "./Engine/System/D3D12System.h"
#include "./Engine/Input/GamePad.h"
#include "./Engine/GameConponent/Camera/Camera.h"
#include "./Engine/Input/InputManager.h"
#include "./Engine/GameConponent/Primitive/Cube.h"
#include "./Engine/Util/Math.h"
#include "./Engine/CommandContext/GraphicsCommandList.h"
#include "./Engine/Signature/RootSignature.h"
#include "./Engine/Audio/Audio.h"
#include "./Engine/Audio/AudioManager.h"
#include "Engine/Resource/StructuredBuffer.h"
#include "Engine/CommandContext/GraphicsContextLibrary.h"

void TestInit();

struct TestVertex {
	unsigned int vertexIndex = 0;
	Vector3 vertex;
	
};

int main() {

	K3D12::SetWindowSize(1280, 720);
	K3D12::SetWindowName(L"1601279_小林開人_D3D12");
	K3D12::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraMode::Orthogonal, 10.0f * Vector3::back, Vector3::zero, Vector3::up, 0.1f, 500.0f);
	K3D12::AudioManager::InitializeXAudio2();

	//auto audio = K3D12::AudioManager::GetInstance().LoadAudio("./audio/An Ordeal From God.wav");

	//audio->Play();

	std::vector<TestVertex> ibuffer;
	ibuffer.resize(10);
	K3D12::StructuredBuffer uav;
	uav.Create(sizeof(TestVertex), 10, &ibuffer[0]);

	TestInit();

	while (K3D12::MessageLoop() == 0 && !K3D12::Input().IsDown(K3D12::VIRTUAL_KEY_STATE::VKS_ESCAPE)) {
		K3D12::Input().InputUpdate();
		K3D12::ClearScreen();

		auto pso = K3D12::D3D12System::GetPSO("ComputePSO");
		auto rs = K3D12::GraphicsContextLibrary::GetInstance().GetRootSignature("ComputePSO");


		K3D12::D3D12System::GetMasterCommandList()->GetCommandList()->SetPipelineState(pso->GetPSO().Get());
		K3D12::D3D12System::GetMasterCommandList()->GetCommandList()->SetComputeRootSignature(rs->GetSignature().Get());
		ID3D12DescriptorHeap* heap[] = { uav.GetHeap()->GetPtr() };

		K3D12::D3D12System::GetMasterCommandList()->GetCommandList()->SetDescriptorHeaps(1, heap);
		K3D12::D3D12System::GetMasterCommandList()->GetCommandList()->SetComputeRootDescriptorTable(0, uav.GetUAVGPUHandle());
		K3D12::D3D12System::GetMasterCommandList()->GetCommandList()->Dispatch(1, 1, 1);
		uav.ReadBack();
		D3D12_RANGE range = { 0,1 };
		uav.Map(0, &range);
		uav.Read(&ibuffer[0], sizeof(TestVertex) * 10);
		uav.Unmap(0,&range);

		K3D12::ScreenFlip();


	}
	//audio->Discard();
		
	K3D12::Destroy();
	return 0;
};

void TestInit() {
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> computeShader;
	Microsoft::WRL::ComPtr<ID3DBlob> error = {};

#if defined(_DEBUG)
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
		;

#else
	UINT compileFlag = 0;
#endif

	if (FAILED(D3DCompileFromFile(L"./Engine/Shader/ComputeShader.hlsl", nullptr, nullptr, "CSMain", "cs_5_0", compileFlag, 0, &computeShader, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	desc.CS.BytecodeLength = computeShader.Get()->GetBufferSize();
	desc.CS.pShaderBytecode = computeShader.Get()->GetBufferPointer();
	desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = nullptr;

	auto result = K3D12::GraphicsContextLibrary::GetInstance().CreatePSO("ComputePSO", desc, computeShader.Get());

	if (FAILED(result)) {
	}
	computeShader.Reset();
	error.Reset();
}