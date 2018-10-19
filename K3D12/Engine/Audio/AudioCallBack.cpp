#include "AudioCallBack.h"



K3D12::AudioCallBack::AudioCallBack() : _handle(CreateEvent(nullptr, false, false, nullptr))
{

}


K3D12::AudioCallBack::~AudioCallBack()
{
	CloseHandle(_handle);
}


void K3D12::AudioCallBack::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{

}

void K3D12::AudioCallBack::OnVoiceProcessingPassEnd(void)
{

}

void K3D12::AudioCallBack::OnStreamEnd(void)
{

}

void K3D12::AudioCallBack::OnBufferStart(void * pBufferContext)
{

}

void K3D12::AudioCallBack::OnBufferEnd(void * pBufferContext)
{
	SetEvent(_handle);
}

void K3D12::AudioCallBack::OnLoopEnd(void * pBufferContext)
{

}

void K3D12::AudioCallBack::OnVoiceError(void * pBufferContext, HRESULT Error)
{

}
