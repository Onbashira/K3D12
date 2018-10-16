#include "AudioCallBack.h"



K3D12::AudioCallBack::AudioCallBack()
{
}


K3D12::AudioCallBack::~AudioCallBack()
{
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
}

void K3D12::AudioCallBack::OnLoopEnd(void * pBufferContext)
{
}

void K3D12::AudioCallBack::OnVoiceError(void * pBufferContext, HRESULT Error)
{
}
