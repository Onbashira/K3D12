#include <vector>
#include <memory>
#include "./Engine/System/D3D12System.h"
#include "./Engine/Input/GamePad.h"
#include "./Engine/GameConponent/Camera/Camera.h"
#include "./Engine/Input/InputManager.h"
#include "./Engine/GameConponent/Primitive/Cube.h"
#include "./Engine/Util/Math.h"
#include "./Engine/Resource/UnorderdAccessView.h"

#include "Engine/CommandContext/GraphicsCommandList.h"
#include "Engine/Signature/RootSignature.h"

int main() {

	K3D12::D3D12System::SetWindowSize(1280, 720);
	K3D12::D3D12System::SetWindowName(L"1601279_小林開人_D3D12");
	K3D12::D3D12System::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraType::Orthogonal, 10.0f * Vector3::back, Vector3::zero, Vector3::up, 0.1f, 500.0f, 0.0f);
	K3D12::D3D12System::InitializeController();



#pragma region CS_TEST
	K3D12::UnorderedAccessValue uav;
	std::vector<float>			testBuffer;
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> computeShader;
	Microsoft::WRL::ComPtr<ID3DBlob> error = {};
	auto cmdList = K3D12::D3D12System::GetMasterCommandList();
	auto queue = K3D12::D3D12System::GetMasterCommandQueue();
	{
#if defined(_DEBUG)
		//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
		UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
		UINT compileFlag = 0;
#endif

		if (FAILED(D3DCompileFromFile(L"./Engine/Shader/ComputeShader.hlsl", nullptr, nullptr, "CSMain", "cs_5_0", compileFlag, 0, &computeShader, &error))) {
			OutputDebugStringA((char*)error->GetBufferPointer());
			return 0;
		}
		if (error != nullptr) {
			OutputDebugStringA((char*)error->GetBufferPointer());
		}

		desc.CS.BytecodeLength = computeShader.Get()->GetBufferSize();
		desc.CS.pShaderBytecode = computeShader.Get()->GetBufferPointer();
		desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = nullptr;

		auto result = K3D12::GraphicsContextLibrary::GetInstance().CreatePSO("ComputeShader", desc, computeShader.Get());

		if (FAILED(result)) {
			return 0;
		}
		computeShader.Reset();
		error.Reset();

	}

	{
		testBuffer.resize(10);
		uav.Create(4,10,&testBuffer[0]);
	}

	cmdList->GetCommandList()->SetComputeRootSignature(K3D12::GraphicsContextLibrary::GetInstance().GetRootSignature("ComputeShader")->GetSignature().Get());
	cmdList->GetCommandList()->SetPipelineState(K3D12::GraphicsContextLibrary::GetInstance().GetPSO("ComputeShader")->GetPSO().Get());
	ID3D12DescriptorHeap* heaps[] = { uav.GetHeap()->GetPtr() };
	cmdList->GetCommandList()->SetDescriptorHeaps(1, heaps);
	cmdList->GetCommandList()->SetComputeRootDescriptorTable(0, uav.GetUAVGPUHandle());
	cmdList->GetCommandList()->Dispatch(1, 1, 1);
	cmdList->CloseCommandList();
	ID3D12CommandList* lists[] = { cmdList->GetCommandList().Get() };
	queue.GetQueue()->ExecuteCommandLists(1, lists);
	K3D12::D3D12System::GetInstance().GetFence()->Wait(&queue);
	cmdList->ResetAllocator();
	cmdList->ResetCommandList();

	uav.Map(0, nullptr);
	auto ptr = uav.GetMappedPointer();

	uav.Read(&testBuffer[0], 4 * 10);
	uav.Unmap(0,nullptr);
#pragma endregion

	












	while (K3D12::D3D12System::MessageLoop() == 0) {
		K3D12::D3D12System::ClearScreen();

		K3D12::D3D12System::ScreenFlip();
	}
	K3D12::D3D12System::Destroy();
	return 0;
};
