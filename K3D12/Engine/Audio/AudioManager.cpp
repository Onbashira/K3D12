#include "AudioManager.h"
#include "../Util/Logger.h"

K3D12::AudioManager::~AudioManager()
{
}


void K3D12::AudioManager::InitializeXAudio2()
{
	HRESULT ret = {};
	if (FAILED(CoInitializeEx(NULL,COINIT_MULTITHREADED))) {
		ERROR_LOG(std::string("AudioManagerの初期化に失敗しました"));
	}

	if (FAILED(XAudio2Create(&_xAudio2))) {
		ERROR_LOG(std::string("XAudio2の作成に失敗しました"));
	};
	auto mv = _masterVoice.get();
	if (FAILED(_xAudio2->CreateMasteringVoice(&mv))) {
		ERROR_LOG(std::string("MasteringVoiceの作成に失敗しました"));
	};
}

void K3D12::AudioManager::Discard()
{
}

void K3D12::AudioManager::StartSoundEngine()
{
}

void K3D12::AudioManager::StopSoundEngine()
{
}
