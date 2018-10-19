#pragma once

#include <vector>
#include <xaudio2.h>

namespace K3D12 {
	enum class WAVE_RESOLUTION_TYPE
	{
		AUDIO_RESOURCE_TYPE_NORMAL_RESOLUTION,
		AUDIO_RESOURCE_TYPE_HIGH_RESOLUTION,
	};
	class AudioWaveSource
	{
		friend class AudioLoader;
	private:

		unsigned int _loadedSize;

		bool _isWaveLoaded;

	public:

	protected:

		WAVE_RESOLUTION_TYPE _resolutionType;

		WAVEFORMATEXTENSIBLE _format;

		std::vector<float>	_wave;	//32bit wave

	private:

	protected:

	public:

		AudioWaveSource();

		virtual ~AudioWaveSource();

		WAVE_RESOLUTION_TYPE GetResolutionType();

		WAVEFORMATEX& GetWaveFormat();

		WAVEFORMATEXTENSIBLE& GetWaveFormatExtensible();

		void CopyWaveAsync(std::vector<float>& wave);

		void CopyWaveBulk(std::vector<float>& wave);

		void SetWave(std::vector<float>& waveForSwap);

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