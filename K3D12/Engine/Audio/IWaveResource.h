#pragma once
#include <string>
#include <memory>

namespace K3D12 {
	enum class WAVE_RESOLUTION_TYPE
	{
		AUDIO_RESOURCE_TYPE_NORMAL_RESOLUTION,
		AUDIO_RESOURCE_TYPE_HIGH_RESOLUTION,
	};
	class IWaveResource
	{
	private:
	public:

	protected:
		WAVE_RESOLUTION_TYPE _resolutionType;
	private:

	protected:

	public:

		IWaveResource();

		virtual ~IWaveResource();

		virtual WAVE_RESOLUTION_TYPE GetResolutionType() = 0;
	};
}

