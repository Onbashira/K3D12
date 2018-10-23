#pragma once

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "winmm.lib")

#include <xaudio2.h>
#include <mmsystem.h>
#include <string>
#include <wrl.h>
#include <memory>
#include "../Util/NonCopyable.h"
#include "AudioCallBack.h"



namespace K3D12 {

	class Audio;
	class AudioWaveSource;
	class AudioCallBack; 

	class AudioManager : private NonCopyable
	{
	private:

		IXAudio2* _xAudio2;

		IXAudio2MasteringVoice* _masterVoice;

	public:

	private:

		AudioManager();
		
		AudioManager(const AudioManager&value) {};
		
		AudioManager(AudioManager&&value) {};
		
		void operator = (const AudioManager& value) {};
		
		void operator = (AudioManager&&value) {};

		std::unique_ptr<Audio> CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);

		std::unique_ptr<Audio> CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);

	public:	
		
		~AudioManager();

		static AudioManager& GetInstance() {
			static AudioManager instance;
			return instance;
		}

		static void InitializeXAudio2();

		void Discard();

		void StartSoundEngine();

		void StopSoundEngine();

		std::unique_ptr<Audio> LoadAudio(std::string audioPath);
	};
}