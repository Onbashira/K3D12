#include "AudioManager.h"
#include "../Util/Logger.h"

K3D12::AudioManager::~AudioManager()
{
	Discard();
}


void K3D12::AudioManager::InitializeXAudio2()
{
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
	if (_xAudio2.Get() != nullptr) {
		_xAudio2.Reset();
	}
	if (_masterVoice != nullptr) {
		_masterVoice.reset();
	}
}

void K3D12::AudioManager::StartSoundEngine()
{
	_xAudio2->StartEngine();
}

void K3D12::AudioManager::StopSoundEngine()
{
	_xAudio2->StopEngine();
}
