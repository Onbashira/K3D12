#include "AudioWaveSource.h"

K3D12::AudioWaveSource::AudioWaveSource() :
	_loadingInfo({}), _format({})
{
}

K3D12::AudioWaveSource::~AudioWaveSource()
{
	this->_wave.clear();
	this->_wave.shrink_to_fit();
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
	return _loadingInfo._isWaveLoaded;
}
