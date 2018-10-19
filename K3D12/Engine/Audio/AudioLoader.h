#pragma once
#include <xaudio2.h>
#include <string>
#include "../Util/NonCopyable.h"
#include "Audio.h"
#include "../AsyncComponent/ThreadPool.h"

namespace K3D12{

	class AudioWaveSource;
	class AudioLoader : public NonCopyable
	{
	private:
		static inline unsigned int _THREAD_NUM = 12;
	public:
	private:

		AudioLoader();
		
		AudioLoader(const AudioLoader&value) {};
		
		AudioLoader(AudioLoader&&value) {};
		
		void operator = (const AudioLoader& value) {};
		
		void operator = (AudioLoader&&value) {};
	
	public:

		~AudioLoader();

		static AudioLoader& GetInstance() {
			static AudioLoader instance;
			return instance;
		}

		std::shared_ptr<AudioWaveSource> LoadAudio(std::string audioFilePath);
		std::shared_ptr<AudioWaveSource> LoadAudioEx(std::string audioFilePath);

	};
}

