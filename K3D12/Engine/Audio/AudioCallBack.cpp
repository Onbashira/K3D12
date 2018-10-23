#include "AudioCallBack.h"



K3D12::AudioCallBack::AudioCallBack() :
	 _onVoiceProcessingPassStart([](UINT32) {})
	, _onVoiceProcessingPassEnd([]() {}), _onStreamEnd([]() {}), _onBufferStart([](void*) {})
	, _onBufferEnd([](void*) {}), _onLoopEnd([](void*) {}), _onVoiceError([](void*,HRESULT) {})

{

}


K3D12::AudioCallBack::~AudioCallBack()
{
}


void K3D12::AudioCallBack::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
	_onVoiceProcessingPassStart(BytesRequired);
}

void K3D12::AudioCallBack::OnVoiceProcessingPassEnd(void)
{
	_onVoiceProcessingPassEnd();
}

void K3D12::AudioCallBack::OnStreamEnd(void)
{
	_onStreamEnd();
}

void K3D12::AudioCallBack::OnBufferStart(void * pBufferContext)
{
	_onBufferStart(pBufferContext);
}

void K3D12::AudioCallBack::OnBufferEnd(void * pBufferContext)
{
	_onBufferEnd(pBufferContext);
}

void K3D12::AudioCallBack::OnLoopEnd(void * pBufferContext)
{
	_onLoopEnd(pBufferContext);
}

void K3D12::AudioCallBack::OnVoiceError(void * pBufferContext, HRESULT Error)
{

	_onVoiceError(pBufferContext, Error);
}

void K3D12::AudioCallBack::SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func)
{
	_onVoiceProcessingPassStart = func;
}

void K3D12::AudioCallBack::SetOnVoiceProcessingPassEnd(std::function<void()> func)
{
	_onVoiceProcessingPassEnd = func;
}

void K3D12::AudioCallBack::SetOnStreamEnd(std::function<void()> func)
{
	_onStreamEnd = func;
}

void K3D12::AudioCallBack::SetOnBufferStart(std::function<void(void*)> func)
{
	_onBufferStart = func;
}

void K3D12::AudioCallBack::SetOnBufferEnd(std::function<void(void*)> func)
{
	_onBufferEnd = func;
}

void K3D12::AudioCallBack::SetOnLoopEnd(std::function<void(void*)> func)
{
	_onLoopEnd = func;
}

void K3D12::AudioCallBack::SetOnVoiceError(std::function<void(void*, HRESULT)> func)
{
	_onVoiceError = func;
}
