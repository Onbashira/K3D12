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

		//�J�n���ɃR�[�������
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

		//�I�����ɃR�[�������
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;

		//�X�g���[���G���h�ɃR�[��
		virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;

		//�o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

		//�o�b�t�@�̏I�[�ŌĂ΂��
		virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

		//���[�v�G���h���ɌĂ΂��
		virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

		//�G���[���N�������Ƃ��ɌĂ΂��
		virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;

		//�J�n���ɃR�[�������
		void  SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func);

		//�I�����ɃR�[�������
		void  SetOnVoiceProcessingPassEnd(std::function<void()> func);

		//�X�g���[���G���h�ɃR�[��
		void  SetOnStreamEnd(std::function<void()> func);

		//�o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		void  SetOnBufferStart(std::function<void(void*)> func);

		//�o�b�t�@�̏I�[�ŌĂ΂��
		void  SetOnBufferEnd(std::function<void(void*)> func);

		//���[�v�G���h���ɌĂ΂��
		void  SetOnLoopEnd(std::function<void(void*)> func);

		//�G���[���N�������Ƃ��ɌĂ΂��
		void  SetOnVoiceError(std::function<void(void*,HRESULT)> func);
	};
}

