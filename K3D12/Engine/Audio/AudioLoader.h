#pragma once
#include <xaudio2.h>
#include <string>
#include "../Util/NonCopyable.h"
#include "Audio.h"

namespace K3D12{

	class IWaveResource;
	class AudioLoader : public NonCopyable
	{
	private:
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

		std::shared_ptr<IWaveResource> LoadAudio(std::string audioFilePath);

	};
}

