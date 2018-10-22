
#include "Audio.h"
#include "AudioWaveSource.h"


K3D12::Audio::Audio() :
	_seekPoint(0),_isLoop(false)
{
}

K3D12::Audio::~Audio()
{
	Discard();
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
	if (_sourceVoice) {
		_sourceVoice->DestroyVoice();
	}
	this->_callBack;
	if (this->_sourceVoice != nullptr) {
		delete _sourceVoice;
	}
	this->_rawData;
}

void K3D12::Audio::SubmitBuffer()
{
	this->_sourceVoice->SubmitSourceBuffer(&this->_audioBuffer);
}
