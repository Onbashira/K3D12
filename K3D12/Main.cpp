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

int main() {

	K3D12::SetWindowSize(1280, 720);
	K3D12::SetWindowName(L"1601279_���ъJ�l_D3D12");
	K3D12::Initialize();
	K3D12::D3D12System::InitializeCamera(K3D12::CameraMode::Orthogonal, 10.0f * Vector3::back, Vector3::zero, Vector3::up, 0.1f, 500.0f);
	K3D12::D3D12System::InitializeController();

	K3D12::AudioManager::InitializeXAudio2();
	bool trig = false;

	auto audio = K3D12::AudioManager::GetInstance().LoadAudio("./audio/An Ordeal From God.wav");

	audio->Play();

	while (K3D12::MessageLoop() == 0 && !K3D12::Input().IsDown(K3D12::VIRTUAL_KEY_STATE::VKS_ESCAPE)) {
		K3D12::Input().InputUpdate();
		K3D12::ClearScreen();

		if (K3D12::Input().IsTriggerDown(K3D12::VIRTUAL_KEY_STATE::VKS_SPACE) && !trig) {
			audio->Stop();
			trig = true;
		}
		else if (K3D12::Input().IsTriggerDown(K3D12::VIRTUAL_KEY_STATE::VKS_SPACE) && trig) {
			audio->Play();
			trig = false;
		}



		K3D12::ScreenFlip();
	}
	audio->Discard();
		
	K3D12::Destroy();
	return 0;
};
