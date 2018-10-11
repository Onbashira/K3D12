#pragma once

#include <xaudio2.h>
#include <string>
#include "../Util/NonCopyable.h"

namespace K3D12 {
	class AudioManager : private NonCopyable
	{
	private:

	public:

	private:

		AudioManager();

	public:	
		
		~AudioManager();

		void LoadMasterVoice();

		void Discard();

		void StartSoundEngine();

		void StopSoundEngine();

	};
}