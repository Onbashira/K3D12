#pragma once
#include <xaudio2.h>
#include <string>
#include "../Util/NonCopyable.h"
#include "AudioRawResource.h"

namespace K3D12{
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

		std::shared_ptr<AudioRawResource> LoadAudio(std::string audioFilePath);

	};
}

