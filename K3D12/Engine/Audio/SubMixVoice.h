#pragma once
#include <xaudio2.h>
#include <memory>

namespace K3D12 {
	class SubMixVoice
	{
	private:
		std::unique_ptr<IXAudio2SourceVoice> _sourceVoice;
	public:
	private:
	public:
		SubMixVoice();
		~SubMixVoice();
	};
}

