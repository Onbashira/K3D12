#pragma once
#include "../Util/NonCopyable.h"
#include "../Util/ManagerComponentBase.h"
namespace K3D12 {
	class AudioWaveSource;
	class AudioSourceManager : private NonCopyable
	{
	private:
		UnorderedManagerComponentBase<AudioWaveSource> _resourceMap;

	public:

	private:

		AudioSourceManager() {};

		AudioSourceManager(const AudioSourceManager&value) {};

		AudioSourceManager(AudioSourceManager&&value) {};

		void operator = (const AudioSourceManager& value) {};

		void operator = (AudioSourceManager&&value) {};

	public:

		~AudioSourceManager() { DiscardManager(); };

		static AudioSourceManager& GetInstance() { static AudioSourceManager instance; return instance; };

		void SetResource(std::string resourceName, std::shared_ptr<AudioWaveSource> source);

		std::weak_ptr<K3D12::AudioWaveSource> GetResource(std::string resourceName);

		void EraseResource(std::string name);

		bool IsLoaded(std::string resourceName);

		void DiscardManager();
	};
}

