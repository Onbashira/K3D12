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
		ERROR_LOG(std::string("XAudio2�̍쐬�Ɏ��s���܂���"));
	};
	INFO_LOG(std::string("XAudio2�̍쐬�ɐ������܂���"));

	if (FAILED(GetInstance()._xAudio2->CreateMasteringVoice(&GetInstance()._masterVoice))) {
		ERROR_LOG(std::string("MasteringVoice�̍쐬�Ɏ��s���܂���"));
	};
	INFO_LOG(std::string("MasteringVoice�̍쐬�ɐ������܂���"));

}

std::unique_ptr<K3D12::Audio> K3D12::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//�ꊇ����
	audio->_rawData = waveResource;
	audio->_audioBuffer.AudioBytes = static_cast<UINT32>(waveResource.lock()->GetWave().size() * sizeof(float));
	audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[0]);
	audio->SubmitBuffer();
	return audio;
}

std::unique_ptr<K3D12::Audio> K3D12::AudioManager::CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//���̃f�[�^�̎Q�Ƃ���������
	audio->_rawData = waveResource;
	//�I�[�ʒu(�ŏI�v�f�ԍ�+1)����
	audio->_audioLength = waveResource.lock()->GetWave().size();
	//�P�b���̃f�[�^�̏������f�[�^���̃o�b�t�@���w�肳���� 
	unsigned int seekValue = waveResource.lock()->GetWaveFormat().nSamplesPerSec * audio->_rawData.lock()->GetWaveFormat().nChannels;

	//��b���̃f�[�^���{�L���[�ɑ���
	for (unsigned int i = 0; i < audio->_callBack.AUDIO_BUFFER_QUEUE_MAX; i++) {

		//�������ȃf�[�^���V�[�N�|�C���g + 1�b�Ԃ̃f�[�^�ʂ���b�����Ȃ�
		if (audio->_seekPoint + seekValue >= audio->_audioLength) {

			audio->_audioBuffer.AudioBytes = static_cast<UINT32>((audio->_audioLength - audio->_seekPoint) * sizeof(float));
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			audio->SubmitBuffer();
			audio->_seekPoint = 0;
			break;
		}
		else {
			//��b�Ԃ̍Đ��ɕK�v�ȃo�C�g��
			audio->_audioBuffer.AudioBytes = static_cast<UINT32>(seekValue * sizeof(float));
			//��b�Ԃ̍Đ����ɕK�v�Ȕg�`�f�[�^�ւ̃|�C���^
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			audio->SubmitBuffer();
		}
		audio->_seekPoint += seekValue;
	}
	auto ptr = audio.get();
	audio->_callBack.SetOnBufferEnd([ptr](void* context) {

		if (ptr->isDiscarded) {
			return;
		}		
		auto state = ptr->GetState();

		//�����L���[���̃o�b�t�@��Q�ݒ萔�l�ȉ��Ȃ�o�b�t�@�ɑ΂��ĐV�����f�[�^����������
		if (state.BuffersQueued < ptr->_callBack.AUDIO_BUFFER_QUEUE_MAX) {

			unsigned int seekValue = ptr->_rawData.lock()->GetWaveFormat().nSamplesPerSec * ptr->_rawData.lock()->GetWaveFormat().nChannels;

			//�������ȃf�[�^���V�[�N�|�C���g + ��b�Ԃ̃f�[�^�ʂ���b�ȉ��������̓��[�v�|�C���g�ɓ��B���Ă���Ȃ�
			if ((ptr->_seekPoint + seekValue) >= ptr->_audioLength) {

				ptr->_audioBuffer.AudioBytes = static_cast<UINT32>((ptr->_audioLength - ptr->_seekPoint) * sizeof(float));
				ptr->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&ptr->_rawData.lock()->GetWave()[ptr->_seekPoint]);
				ptr->SubmitBuffer();
				if (ptr->_isLoop == false) {
					ptr->Stop();
				}
				ptr->_seekPoint = 0;
				return;
			}
			else {
				ptr->_audioBuffer.AudioBytes = static_cast<UINT32>(seekValue * sizeof(float));
				ptr->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&ptr->_rawData.lock()->GetWave()[ptr->_seekPoint]);
				ptr->_seekPoint += seekValue;

				ptr->SubmitBuffer();
			}

		}
	});
	//�T�u�~�b�g
	return std::move(audio);
}

void K3D12::AudioManager::Discard()
{
	AudioLoader::GetInstance().StopLoad();
	AudioLoader::GetInstance().DiscardWorkerThreads();

	if (_masterVoice != nullptr) {
		_masterVoice->DestroyVoice();
		_masterVoice = nullptr;
		INFO_LOG(std::string("MasteringVoice���폜���܂���"));
	}
	if (_xAudio2 != nullptr) {
		_xAudio2->Release();
		_xAudio2 = nullptr;
		INFO_LOG(std::string("XAudio2���폜���܂���"));
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
