#include "AudioManager.h"
#include "../Util/Logger.h"
#include "AudioLoader.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"

K3D12::AudioManager::AudioManager()
{


}

K3D12::AudioManager::~AudioManager()
{
	Discard();
}



void K3D12::AudioManager::InitializeXAudio2()
{
	if (FAILED(XAudio2Create(&GetInstance()._xAudio2))) {
		ERROR_LOG(std::string("XAudio2ÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩ"));
	};
	INFO_LOG(std::string("XAudio2ÇÃçÏê¨Ç…ê¨å˜ÇµÇ‹ÇµÇΩ"));
	auto mv = GetInstance()._masterVoice.get();
	if (FAILED(GetInstance()._xAudio2->CreateMasteringVoice(&mv))) {
		ERROR_LOG(std::string("MasteringVoiceÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩ"));
	};
	INFO_LOG(std::string("MasteringVoiceÇÃçÏê¨Ç…ê¨å˜ÇµÇ‹ÇµÇΩ"));

}

K3D12::Audio K3D12::AudioManager::CreateSourceVoice(AudioWaveSource & waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	return Audio();
}

K3D12::Audio K3D12::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	Audio audio;

	auto sourceVoicePtr = audio._sourceVoice.get();
	_xAudio2->CreateSourceVoice(&(sourceVoicePtr), &waveResource.lock()->GetWaveFormat(), 0, 2.0f, callback, sendList, effectChain);


	audio._callBack = *callback;
	audio._audioBuffer.AudioBytes = waveResource.lock()->GetWave().size();
	audio._audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[0]);

	return audio;
}

void K3D12::AudioManager::Discard()
{
	if (_xAudio2.Get() != nullptr) {
		_xAudio2.Reset();
		INFO_LOG(std::string("MasteringVoiceÇçÌèúÇµÇ‹ÇµÇΩ"));

	}
	if (_masterVoice != nullptr) {
		_masterVoice.reset();
		INFO_LOG(std::string("MasteringVoiceÇçÌèúÇµÇ‹ÇµÇΩ"));

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

K3D12::Audio K3D12::AudioManager::LoadAudio(std::string audioPath)
{

	auto audioResource = AudioLoader::GetInstance().LoadAudio(audioPath);


	Audio audio = this->CreateSourceVoice(audioResource);;

	return Audio();
}
