#pragma once

#include <xaudio2.h>

#include <string>

namespace K3D12 {
	class IAudio
	{
		friend class AudioManager;

		friend class AudioLoader;

	private:

	protected:

		std::weak_ptr<IXAudio2SourceVoice> _sourceVoice;

		unsigned int _loopPoint;

		bool _isPlay;

		std::string _soundName;

	public:

	private:

		void SetVoiceSource(std::weak_ptr<IXAudio2SourceVoice> sourceVoice);

	public:

		IAudio();

		virtual ~IAudio();

		void Stop();

		void Play();

		void SetLoopPoint(unsigned int frame);

		bool IsPlay();

		void DiscardMasterVoice();
	};
}

