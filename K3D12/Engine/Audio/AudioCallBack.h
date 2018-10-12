#pragma once
#include <xaudio2.h>
class AudioCallBack : public IXAudio2VoiceCallback
{
private:

protected:

public:

private:

protected:

public:

	AudioCallBack();

	virtual ~AudioCallBack();

	virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

	virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;

	virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;

	virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

	virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

	virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

	virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;
};

