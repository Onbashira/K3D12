#pragma once

#include <xaudio2.h>
#include <memory>
#include <ks.h>
#include "AudioCallBack.h"

namespace K3D12 {

	class SubMixVoice;
	class IWaveResource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;
	private:
		//サウンドごとのデータ
		XAUDIO2_BUFFER _audioBuffer;

		std::unique_ptr<IXAudio2SourceVoice> _sourceVoice;
		
		std::weak_ptr<IWaveResource> _rawData;

		AudioCallBack _callBack;

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

		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

	};
}