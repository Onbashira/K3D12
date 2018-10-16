#pragma once
#include <xaudio2.h>
#include <memory>

namespace K3D12 {
	class SubMixVoice
	{
		friend class SubMicVoiceManager;
	private:
		std::unique_ptr<IXAudio2SubmixVoice> _subMixVoice;
	public:
	private:
	public:
		SubMixVoice();
		~SubMixVoice();
	};
}

