#include <vector>
#include <memory>
#include "./Engine/System/D3D12System.h"
#include "./Engine/Input/GamePad.h"
#include "./Engine/GameConponent/Camera/Camera.h"
#include "./Engine/Input/InputManager.h"
#include "./Engine/GameConponent/Primitive/Cube.h"
#include "./Engine/Util/Math.h"
#include "./Engine/Resource/UnorderedAccessValue.h"

#include "Engine/CommandContext/GraphicsCommandList.h"
#include "Engine/Signature/RootSignature.h"

int main() {

	K3D12::SetWindowSize(1280, 720);
	K3D12::SetWindowName(L"1601279_è¨ó—äJêl_D3D12");
	K3D12::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraType::Orthogonal, 10.0f * Vector3::back, Vector3::zero, Vector3::up, 0.1f, 500.0f, 0.0f);
	K3D12::D3D12System::InitializeController();

	while (K3D12::MessageLoop() == 0 || K3D12::Input().IsTriggerDown(K3D12::VIRTUAL_KEY_STATE::VKS_ESCAPE)) {
		K3D12::ClearScreen();

		K3D12::ScreenFlip();
	}
	K3D12::Destroy();
	return 0;
};
