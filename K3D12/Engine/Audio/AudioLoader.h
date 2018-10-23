#pragma once
#include <memory>
#include <string>
#include "../Util/NonCopyable.h"
#include "Audio.h"
#include "../AsyncComponent/ThreadPool.h"

namespace K3D12{

	class AudioWaveSource;
	class AudioLoader : public NonCopyable
	{
	private:
		static inline unsigned int _THREAD_NUM = 8;
		
		ThreadPool _loadThreadPool;

		bool _isStop;

	public:
	private:

		AudioLoader() : _loadThreadPool(_THREAD_NUM) {};
		
		AudioLoader(const AudioLoader&value) : _loadThreadPool(_THREAD_NUM) {};
		
		AudioLoader(AudioLoader&&value) : _loadThreadPool(_THREAD_NUM) {};
		
		void operator = (const AudioLoader& value) {};
		
		void operator = (AudioLoader&&value) {};
	
	public:

		~AudioLoader() {};

		static AudioLoader& GetInstance() {
			static AudioLoader instance;
			return instance;
		}

		void StopLoad();

		void ReStartLoad();

		bool IsStopLoad();

		void DiscardWorkerThreads();

		std::shared_ptr<AudioWaveSource> LoadAudio(std::string audioFilePath);

		std::shared_ptr<AudioWaveSource> LoadAudioEx(std::string audioFilePath);

	};
}

