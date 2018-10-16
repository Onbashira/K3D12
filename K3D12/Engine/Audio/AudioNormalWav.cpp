#include "AudioNormalWav.h"

K3D12::AudioNormalWav::AudioNormalWav()
{
}

K3D12::AudioNormalWav::~AudioNormalWav()
{
	this->_wave.clear();
	this->_wave.shrink_to_fit();
}


K3D12::WAVE_RESOLUTION_TYPE K3D12::AudioNormalWav::GetResolutionType()
{
	return _resolutionType;
}

WAVEFORMATEX & K3D12::AudioNormalWav::GetWaveFormat()
{
	return _waveFormat;
}

std::vector<short>& K3D12::AudioNormalWav::GetWave()
{
	return _wave;
}
