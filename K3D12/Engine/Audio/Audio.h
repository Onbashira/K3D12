#pragma once

#include <xaudio2.h>
#include <memory>
#include <deque>
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
		//サウンドごとのデータ
		XAUDIO2_BUFFER _audioBuffer;

		IXAudio2SourceVoice* _sourceVoice;
		
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

		//Audio(const Audio& other);

		//Audio(Audio&& other);

		virtual ~Audio();

		//Audio& operator=(const Audio& other);

		//Audio& operator=(Audio&& other);

		virtual void Play();

		virtual void Stop();

		void LoopEnable();

		void LoopDisable();

		void SetLoopPoint();

		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

		//再生キューにデータを追加
		void SubmitBuffer();

	};
}