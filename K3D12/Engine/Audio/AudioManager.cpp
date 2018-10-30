#include "AudioManager.h"
#include "../Util/Logger.h"
#include "AudioLoader.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"
#include "Audio.h"

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

std::unique_ptr<K3D12::Audio> K3D12::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//一括送り
	audio->_rawData = waveResource;
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());

	audio->BulkSubmit();
	return audio;
}

std::unique_ptr<K3D12::Audio> K3D12::AudioManager::CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//生のデータの参照を持たせる
	audio->_rawData = waveResource;
	//終端位置(最終要素番号+1)を代入 
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());

	//１秒分のデータの情報を持つデータ分のバッファを指定させる 一秒間に必要なサンプリング数*チャンネル数
	unsigned int seekValue = waveResource.lock()->GetWaveFormat().nSamplesPerSec * waveResource.lock()->GetWaveFormat().nChannels;
	unsigned int audioBytePerSec = waveResource.lock()->GetWaveFormat().nAvgBytesPerSec;
	//一秒分のデータを二本キューに送る
	for (unsigned int i = 0; i < audio->_callBack.AUDIO_BUFFER_QUEUE_MAX; i++) {

		//もしも曲データがシークポイント + 1秒間のデータ量が一秒未満なら
		if (audio->_seekPoint + seekValue >= audio->_audioLength) {

			audio->_audioBuffer.AudioBytes = static_cast<UINT32>((audio->_audioLength - audio->_seekPoint) * sizeof(float));
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			//std::stringstream ss;
			//ss << static_cast<float>((float)audio->_seekPoint / (float)audio->_audioLength) * 100.0f << " % ";
			//DETAILS_LOG(ss.str());

			audio->SubmitBuffer();
			audio->_seekPoint = 0;
			break;
		}
		else {
			//一秒間の再生に必要なバイト数
			audio->_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
			//一秒間の再生いに必要な波形データへのポインタ
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			//std::stringstream ss;
			//ss << static_cast<float>((float)audio->_seekPoint / (float)audio->_audioLength) * 100.0f << " % ";
			//DETAILS_LOG(ss.str());

			audio->SubmitBuffer();
		}
		audio->_seekPoint += seekValue;
	}
	audio->StreamSubmit();
	//サブミット
	return std::move(audio);
}

void K3D12::AudioManager::Discard()
{
	AudioLoader::GetInstance().StopLoad();
	AudioLoader::GetInstance().DiscardWorkerThreads();

	if (_masterVoice != nullptr) {
		_masterVoice->DestroyVoice();
		_masterVoice = nullptr;
		INFO_LOG(std::string("MasteringVoiceを削除しました"));
	}
	if (_xAudio2 != nullptr) {
		_xAudio2->Release();
		_xAudio2 = nullptr;
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

std::unique_ptr<K3D12::Audio> K3D12::AudioManager::LoadAudio(std::string audioPath)
{
	auto audioResource = AudioLoader::GetInstance().LoadAudioEx(audioPath);

	auto audio = std::move(this->CreateSourceVoiceEx(audioResource));

	return std::move(audio);;
}
