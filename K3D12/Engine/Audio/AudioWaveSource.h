#pragma once

#include <vector>
#include <xaudio2.h>

namespace K3D12 {
	class AudioWaveSource
	{
		friend class AudioLoader;
	private:

		unsigned int _loadedSize;

		bool _isWaveLoaded;

	public:

	protected:

		WAVEFORMATEXTENSIBLE _format;

		std::vector<float>	_wave;	//32bit wave

	private:

	protected:

	public:

		AudioWaveSource();

		virtual ~AudioWaveSource();

		WAVEFORMATEX& GetWaveFormat();

		WAVEFORMATEXTENSIBLE& GetWaveFormatExtensible();

		//void CopyWaveAsync(std::vector<float>& wave);

		//void CopyWaveBulk(std::vector<float>& wave);

		//void SetWave(std::vector<float>& waveForSwap);

		std::vector<float>&	GetWave();

		bool IsWaveLoaded();
	};
}

struct Stereo8Bit {
	unsigned char left;
	unsigned char right;
};

struct Stereo16Bit {
	short left;
	short right;
};