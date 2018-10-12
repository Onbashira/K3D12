#pragma once
#include <vector>
namespace K3D12 {
	struct AudioRawResource
	{
		std::vector<short>	rawAudioResource;
		
		WAVEFORMATEX		waveFormat;
		
		AudioRawResource();
		
		~AudioRawResource();

	};
}

