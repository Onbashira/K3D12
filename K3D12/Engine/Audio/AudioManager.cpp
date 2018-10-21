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
		ERROR_LOG(std::string("XAudio2‚Ìì¬‚É¸”s‚µ‚Ü‚µ‚½"));
	};
	INFO_LOG(std::string("XAudio2‚Ìì¬‚É¬Œ÷‚µ‚Ü‚µ‚½"));
	auto mv = GetInstance()._masterVoice.get();
	if (FAILED(GetInstance()._xAudio2->CreateMasteringVoice(&mv))) {
		ERROR_LOG(std::string("MasteringVoice‚Ìì¬‚É¸”s‚µ‚Ü‚µ‚½"));
	};
	INFO_LOG(std::string("MasteringVoice‚Ìì¬‚É¬Œ÷‚µ‚Ü‚µ‚½"));

}

K3D12::Audio K3D12::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	Audio audio;

	if (callback != nullptr) {
		audio._callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio._sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio._callBack, sendList, effectChain);


	audio._audioBuffer.AudioBytes = waveResource.lock()->GetWave().size();
	audio._audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[0]);
	audio.SubmitBuffer();
	return audio;
}

void K3D12::AudioManager::Discard()
{
	if (_xAudio2.Get() != nullptr) {
		_xAudio2.Reset();
		INFO_LOG(std::string("MasteringVoice‚ğíœ‚µ‚Ü‚µ‚½"));

	}
	if (_masterVoice != nullptr) {
		_masterVoice.reset();
		INFO_LOG(std::string("MasteringVoice‚ğíœ‚µ‚Ü‚µ‚½"));

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
