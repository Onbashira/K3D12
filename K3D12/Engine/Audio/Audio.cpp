#include "Audio.h"



K3D12::Audio::Audio()
{
}

K3D12::Audio::Audio(const Audio & other)
{
	*this = other;
}

K3D12::Audio::~Audio()
{
}

K3D12::Audio & K3D12::Audio::operator=(const Audio & other)
{
	this->_audioBuffer = other._audioBuffer;
	this->_callBack = other._callBack;
	this->_rawData.reset();
	this->_sourceVoice.reset(other._sourceVoice.get());
	return  *this;
}

void K3D12::Audio::Play()
{
	this->_sourceVoice->Start();
}

void K3D12::Audio::Stop()
{
	this->_sourceVoice->Stop();
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
	if (_sourceVoice) {
		_sourceVoice->DestroyVoice();
	}
	this->_callBack;
	this->_sourceVoice.reset();
	this->_rawData;
}
