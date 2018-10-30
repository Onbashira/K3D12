
#include <iomanip>
#include <sstream>
#include "Audio.h"
#include "AudioWaveSource.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"
#include "../Util/Logger.h"


K3D12::Audio::Audio() :
	_seekPoint(0), _isLoop(false), _isDiscarded(false)
{
}

K3D12::Audio::~Audio()
{
	Discard();
}


void K3D12::Audio::BulkSubmit()
{

	_seekPoint = 0;

	_audioBuffer.AudioBytes = static_cast<UINT32>(_rawData.lock()->GetWaveFormat().nAvgBytesPerSec);
	_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[0]);
	SubmitBuffer();

	this->_callBack.SetOnBufferEnd([this](void* context) {
		if (_isDiscarded) {
			this->_callBack.SetOnBufferEnd([](void* context) {});
			return;
		}
		_audioBuffer.AudioBytes = static_cast<UINT32>(_rawData.lock()->GetWave().size() * sizeof(float));
		_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[0]);
		if (_isLoop == false) {
			Stop();
		}
		SubmitBuffer();

	});
}

void K3D12::Audio::StreamSubmit()
{
	_callBack.SetOnBufferEnd([this](void* context) {

		if (_isDiscarded) {
			this->_callBack.SetOnBufferEnd([](void* context) {});
			return;
		}
		auto state = GetState();

		//もしキュー内のバッファがQ設定数値以下ならバッファに対して新しいデータを供給する
		if (state.BuffersQueued < _callBack.AUDIO_BUFFER_QUEUE_MAX) {
			// 44.1k * byte * channel
			unsigned int seekValue = _rawData.lock()->GetWaveFormat().nSamplesPerSec * _rawData.lock()->GetWaveFormat().nChannels;
			unsigned int audioBytePerSec = _rawData.lock()->GetWaveFormat().nAvgBytesPerSec;
			if (_seekPoint >= _audioLength) {
				_seekPoint = 0;
				_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
				_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[_seekPoint]);
				SubmitBuffer();
				_seekPoint += seekValue;
				if (_isLoop == false) {
					Stop();
				}
				return;
			}


			//もしも曲データがシークポイント + 一秒間のデータ量が一秒以下もしくはループポイントに到達しているなら
			if ((_seekPoint + seekValue) >= _audioLength) {
				unsigned int  byte = _audioLength - _seekPoint;
				//全体のバイト数　－　現在のサンプル点
				_audioBuffer.AudioBytes = static_cast<UINT32>((byte) * sizeof(float));
				_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[_seekPoint]);
				SubmitBuffer();

				_seekPoint += byte;
				return;
			}
			else {
				_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
				_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[_seekPoint]);

				SubmitBuffer();
				_seekPoint += seekValue;

			}
		}
	});
}

void K3D12::Audio::Play()
{
	this->_sourceVoice->Start();
}

void K3D12::Audio::Stop()
{
	this->_sourceVoice->Stop();
}

void K3D12::Audio::LoopEnable()
{
	_isLoop = true;
}

void K3D12::Audio::LoopDisable()
{
	_isLoop = false;
}

void K3D12::Audio::SetLoopPoint(unsigned int loopPoint)
{
	LoopEnable();
}

XAUDIO2_VOICE_STATE K3D12::Audio::GetState()
{
	_sourceVoice->GetState(&_voiceState);
	return this->_voiceState;
}

void K3D12::Audio::Pause(bool pause)
{
	if (pause) {
		_sourceVoice->Stop();
	}
	else {
		_sourceVoice->Start();
	}
}

bool K3D12::Audio::CheckPlaying()
{
	XAUDIO2_VOICE_STATE state;
	_sourceVoice->GetState(&state);
	return state.BuffersQueued > 0;
}

void K3D12::Audio::Discard()
{
	if (this->_sourceVoice != nullptr) {
		Stop();
		_sourceVoice->FlushSourceBuffers();
		_sourceVoice->Discontinuity();
		_sourceVoice->DestroyVoice();
		//ヌルポインタの代入
		_sourceVoice = nullptr;
		//ソースボイスは消していけない・・・？
		//delete _sourceVoice;
	}
	_isDiscarded = true;
	this->_rawData.reset();
}

void K3D12::Audio::SubmitBuffer()
{
	this->_sourceVoice->SubmitSourceBuffer(&this->_audioBuffer);
}
