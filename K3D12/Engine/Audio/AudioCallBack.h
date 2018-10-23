#pragma once
#include <xaudio2.h>
#include <functional>
#include <map>

namespace K3D12 {
	class AudioCallBack : public IXAudio2VoiceCallback
	{
	private:

	protected:
				
		std::function<void(UINT32)> _onVoiceProcessingPassStart;

		std::function<void()> _onVoiceProcessingPassEnd;

		std::function<void()> _onStreamEnd;

		std::function<void(void*)> _onBufferStart;

		std::function<void(void*)> _onBufferEnd;

		std::function<void(void*)> _onLoopEnd;

		std::function<void(void*, HRESULT)> _onVoiceError;

	public:
		static inline unsigned int AUDIO_BUFFER_QUEUE_MAX = 2;
	private:

	protected:

	public:

		AudioCallBack();

		virtual ~AudioCallBack();

		//開始時にコールされる
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

		//終了時にコールされる
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;

		//ストリームエンドにコール
		virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;

		//バッファスタートの時に呼ばれる
		virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

		//バッファの終端で呼ばれる
		virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

		//ループエンド時に呼ばれる
		virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

		//エラーを起こしたときに呼ばれる
		virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;

		//開始時にコールされる
		void  SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func);

		//終了時にコールされる
		void  SetOnVoiceProcessingPassEnd(std::function<void()> func);

		//ストリームエンドにコール
		void  SetOnStreamEnd(std::function<void()> func);

		//バッファスタートの時に呼ばれる
		void  SetOnBufferStart(std::function<void(void*)> func);

		//バッファの終端で呼ばれる
		void  SetOnBufferEnd(std::function<void(void*)> func);

		//ループエンド時に呼ばれる
		void  SetOnLoopEnd(std::function<void(void*)> func);

		//エラーを起こしたときに呼ばれる
		void  SetOnVoiceError(std::function<void(void*,HRESULT)> func);
	};
}

