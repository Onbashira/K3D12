#pragma once

#include <xaudio2.h>
#include <memory>
#include <ks.h>
#include <ksarm64.h>
#include "AudioCallBack.h"
#include "AudioRawResource.h"

namespace K3D12 {

	class IAudioSource
	{
		friend class AudioManager;
	private:
		//サウンドごとのデータ
		std::unique_ptr<IXAudio2SourceVoice> _sourceVoice;
		
		AudioRawResource _rawData;

		std::unique_ptr<AudioCallBack> _callBack;

	public:

	private:

	protected:
		
	public:
		
		IAudioSource();

		virtual ~IAudioSource();

		virtual void Play();

		virtual void Stop();

		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

	};
}