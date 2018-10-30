#pragma once

#include <xaudio2.h>
#include <memory>
#include <array>
#include <deque>
#include "AudioCallBack.h"

namespace K3D12 {

	class SubMixVoice;
	class AudioWaveSource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;

	private:
		//�T�E���h���Ƃ̃f�[�^
		XAUDIO2_BUFFER _audioBuffer;

		IXAudio2SourceVoice* _sourceVoice;
		
		std::weak_ptr<AudioWaveSource> _rawData;

		AudioCallBack _callBack;

		XAUDIO2_VOICE_STATE _voiceState;

		WAVEFORMATEXTENSIBLE _format;

		//���f�[�^�̂ǂ̈ʒu�����b�Ԃ̃T���v�����O���s���Ă��邩��m�点��v�f�ԍ�
		unsigned int _seekPoint;

		//�Ȃ̍ő�Ȃ����B���[�v�|�C���g��ݒ肷��Ƃ��̒������ς��B�f�t�H���g�Ō��������̃|�C���g
		unsigned int _audioLength;

		//���[�v���邩���Ȃ����B
		bool _isLoop;

		bool _isDiscarded;

	public:

	private:

	protected:

		void BulkSubmit();

		void StreamSubmit();

	public:
		
		Audio();

		virtual ~Audio();

		virtual void Play();

		virtual void Stop();

		void LoopEnable();

		void LoopDisable();

		void SetLoopPoint(unsigned int loopPoint = 0);

		XAUDIO2_VOICE_STATE GetState();

		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

		//�Đ��L���[�Ƀf�[�^��ǉ�
		void SubmitBuffer();

	};
}