#pragma once

#include <xaudio2.h>
#include <memory>
#include <deque>
#include <ks.h>
#include <future>
#include "AudioCallBack.h"

namespace K3D12 {

	class SubMixVoice;
	class AudioWaveResource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;
	private:
		//�T�E���h���Ƃ̃f�[�^
		XAUDIO2_BUFFER _audioBuffer;

		std::unique_ptr<IXAudio2SourceVoice> _sourceVoice;
		
		std::weak_ptr<AudioWaveResource> _rawData;

		AudioCallBack _callBack;

		XAUDIO2_VOICE_STATE _voiceState;

		WAVEFORMATEXTENSIBLE _format;

		std::deque<float*> _wavePointer;

		unsigned int _currentWaveIndex;

	public:

	private:

	protected:

	public:
		
		Audio();

		Audio(const Audio& other);

		virtual ~Audio();

		Audio& operator=(const Audio& other);

		virtual void Play();

		virtual void Stop();

		void LoopEnable();

		void LoopDisable();

		void SetLoopPoint();



		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

	};
}