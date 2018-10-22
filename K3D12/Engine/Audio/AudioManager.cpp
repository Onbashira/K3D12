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
		ERROR_LOG(std::string("XAudio2の作成に失敗しました"));
	};
	INFO_LOG(std::string("XAudio2の作成に成功しました"));

	if (FAILED(GetInstance()._xAudio2->CreateMasteringVoice(&GetInstance()._masterVoice))) {
		ERROR_LOG(std::string("MasteringVoiceの作成に失敗しました"));
	};
	INFO_LOG(std::string("MasteringVoiceの作成に成功しました"));

}

K3D12::Audio* K3D12::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	Audio* audio = new Audio();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//一括送り
	audio->_rawData = waveResource;
	audio->_audioBuffer.AudioBytes = static_cast<UINT32>(waveResource.lock()->GetWave().size() * sizeof(float));
	audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[0]);
	audio->SubmitBuffer();
	return audio;
}

K3D12::Audio * K3D12::AudioManager::CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	Audio* audio = new Audio();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//生のデータの参照を持たせる
	audio->_rawData = waveResource;
	//終端位置(最終要素番号+1)を代入
	audio->_audioLength = waveResource.lock()->GetWave().size();
	//１秒分のデータの情報を持つデータ分のバッファを指定させる 
	unsigned int seekValue = waveResource.lock()->GetWaveFormat().nSamplesPerSec * audio->_rawData.lock()->GetWaveFormat().nChannels;

	//一秒分のデータを二本キューに送る
	for (int i = 0; i < audio->_callBack.AUDIO_BUFFER_QUEUE_MAX; i++) {

		//もしも曲データがシークポイント + 1秒間のデータ量が一秒未満なら
		if (audio->_seekPoint + seekValue >= audio->_audioLength) {

			audio->_audioBuffer.AudioBytes = static_cast<UINT32>((audio->_audioLength - audio->_seekPoint) * sizeof(float));
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			audio->SubmitBuffer();
			audio->_seekPoint = 0;
			break;
		}
		else {
			//一秒間の再生に必要なバイト数
			audio->_audioBuffer.AudioBytes = static_cast<UINT32>(seekValue * sizeof(float));
			//一秒間の再生いに必要な波形データへのポインタ
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			audio->SubmitBuffer();
		}
		audio->_seekPoint += seekValue;
	}

	audio->_callBack.SetOnBufferEnd([&audio](void* context) {
		auto state = audio->GetState();

		//もしキュー内のバッファがQ設定数値以下ならバッファに対して新しいデータを供給する
		if (state.BuffersQueued < audio->_callBack.AUDIO_BUFFER_QUEUE_MAX) {

			unsigned int seekValue = audio->_rawData.lock()->GetWaveFormat().nSamplesPerSec * audio->_rawData.lock()->GetWaveFormat().nChannels;

			//もしも曲データがシークポイント + 一秒間のデータ量が一秒以下もしくはループポイントに到達しているなら
			if ((audio->_seekPoint + seekValue) >= audio->_audioLength) {

				audio->_audioBuffer.AudioBytes = static_cast<UINT32>((audio->_audioLength - audio->_seekPoint) * sizeof(float));
				audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&audio->_rawData.lock()->GetWave()[audio->_seekPoint]);
				audio->SubmitBuffer();
				if (audio->_isLoop == false) {
					audio->Stop();
				}
				audio->_seekPoint = 0;

				return;
			}
			else {
				audio->_audioBuffer.AudioBytes = static_cast<UINT32>(seekValue * sizeof(float));
				audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&audio->_rawData.lock()->GetWave()[audio->_seekPoint]);
				audio->_seekPoint += audio->_rawData.lock()->GetWaveFormat().nSamplesPerSec;

				audio->SubmitBuffer();
			}

		}
	});
	//サブミット
	return audio;
}

void K3D12::AudioManager::Discard()
{
	if (_masterVoice != nullptr) {
		_masterVoice->DestroyVoice();
		_masterVoice = nullptr;
		INFO_LOG(std::string("MasteringVoiceを削除しました"));
	}
	if (_xAudio2.Get() != nullptr) {
		_xAudio2->Release();
		INFO_LOG(std::string("XAudio2を削除しました"));
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

K3D12::Audio* K3D12::AudioManager::LoadAudio(std::string audioPath)
{

	auto audioResource = AudioLoader::GetInstance().LoadAudioEx(audioPath);


	Audio* audio = this->CreateSourceVoiceEx(audioResource);;

	return audio;
}
