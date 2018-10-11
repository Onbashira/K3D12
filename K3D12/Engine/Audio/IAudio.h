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

		unsigned int _loopPoint;

		bool _isPlay;

		std::string _soundName;

	public:

	private:

	public:

		IAudio();

		virtual ~IAudio();

		void Stop();

		void Play();

		void SetLoopPoint(unsigned int frame);

		bool IsPlay();
	};
}

