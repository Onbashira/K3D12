#include "AudioWaveSource.h"

K3D12::AudioWaveSource::AudioWaveSource() :
	_loadedSize(0), _isWaveLoaded(false), _format({})
{
}

K3D12::AudioWaveSource::~AudioWaveSource()
{
	this->_wave.clear();
	this->_wave.shrink_to_fit();
}


K3D12::WAVE_RESOLUTION_TYPE K3D12::AudioWaveSource::GetResolutionType()
{
	return _resolutionType;
}


WAVEFORMATEX & K3D12::AudioWaveSource::GetWaveFormat()
{
	return _format.Format;
}

WAVEFORMATEXTENSIBLE & K3D12::AudioWaveSource::GetWaveFormatExtensible()
{
	return _format;
}

std::vector<float>& K3D12::AudioWaveSource::GetWave()
{
	return _wave;
}

bool K3D12::AudioWaveSource::IsWaveLoaded()
{
	return _isWaveLoaded;
}
