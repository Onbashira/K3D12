#include <vector>
#include "./Engine/System/D3D12System.h"
#include "./Engine/Input/GamePad.h"
#include "./Engine/GameConponent/Camera/Camera.h"
#include "./Engine/Input/InputManager.h"
#include "./Engine/GameConponent/Primitive/Cube.h"
#include "./Engine/Util/Math.h"
#include "./Engine/Resource/UnorderdAccessView.h"
#include "Engine/GameConponent/Primitive/Cube.h"
int main() {

	K3D12::D3D12System::SetWindowSize(1280, 720);
	K3D12::D3D12System::SetWindowName(L"1601279_���ъJ�l_D3D12");
	K3D12::D3D12System::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraType::Orthogonal, 10.0f * Vector3::back, Vector3::zero, Vector3::up, 0.1f, 500.0f, 0.0f);
	K3D12::D3D12System::InitializeController();

	while (K3D12::D3D12System::MessageLoop() == 0) {
		K3D12::D3D12System::ClearScreen();

		K3D12::D3D12System::ScreenFlip();
	}
	K3D12::D3D12System::Destroy();
	return 0;
};
