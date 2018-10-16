#pragma once

#include <vector>
#include <xaudio2.h>
#include "Audio.h"
#include "IWaveResource.h"


namespace K3D12 {
	class AudioNormalWav : public IWaveResource
	{
		friend class AudioLoader;
	private:

		WAVEFORMATEX		_waveFormat;

		std::vector<short>	_wave;

	public:

	private:

	public:

		AudioNormalWav();

		virtual ~AudioNormalWav();

		WAVE_RESOLUTION_TYPE GetResolutionType()override;

		WAVEFORMATEX& GetWaveFormat();

		std::vector<short>& GetWave();
	};
}