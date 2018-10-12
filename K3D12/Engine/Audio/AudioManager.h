#pragma once

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "winmm.lib")

#include <xaudio2.h>
#include <mmsystem.h>
#include <string>
#include <wrl.h>
#include "../Util/NonCopyable.h"



namespace K3D12 {
	class AudioManager : private NonCopyable
	{
	private:

		Microsoft::WRL::ComPtr<IXAudio2> _xAudio2;

		std::unique_ptr<IXAudio2MasteringVoice> _masterVoice;

	public:

	private:

		AudioManager();
		
		AudioManager(const AudioManager&value) {};
		
		AudioManager(AudioManager&&value) {};
		
		void operator = (const AudioManager& value) {};
		
		void operator = (AudioManager&&value) {};
		
		void InitializeXAudio2();

	public:	
		
		~AudioManager();

		void Discard();

		void StartSoundEngine();

		void StopSoundEngine();

	};
}