#include "IAudioSource.h"



K3D12::IAudioSource::IAudioSource()
{
}

K3D12::IAudioSource::~IAudioSource()
{
}

void K3D12::IAudioSource::Play()
{
	this->_sourceVoice->Start();
}

void K3D12::IAudioSource::Stop()
{
	this->_sourceVoice->Stop();
}

void K3D12::IAudioSource::Pause(bool pause)
{
	if (pause) {
		_sourceVoice->Stop();
	}
	else {
		_sourceVoice->Start();
	}
}

bool K3D12::IAudioSource::CheckPlaying()
{
	XAUDIO2_VOICE_STATE state;
	_sourceVoice->GetState(&state);
	return state.BuffersQueued > 0;
}

void K3D12::IAudioSource::Discard()
{
	if (_sourceVoice) {
		_sourceVoice->DestroyVoice();
	}
	this->_callBack.reset();
	this->_sourceVoice.reset();
	this->_rawData;
}
