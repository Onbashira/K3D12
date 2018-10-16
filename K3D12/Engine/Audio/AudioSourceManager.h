#pragma once
#include "../Util/NonCopyable.h"
#include "../Util/ManagerComponentBase.h"
namespace K3D12 {
	class IWaveResource;
	class AudioSourceManager : private NonCopyable
	{
	private:
		UnorderedManagerComponentBase<IWaveResource> _resourceMap;

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

		void SetResource(std::string resourceName, std::shared_ptr<IWaveResource> source);

		std::weak_ptr<K3D12::IWaveResource> GetResource(std::string resourceName);

		void EraseResource(std::string name);

		bool IsLoaded(std::string resourceName);

		void DiscardManager();
	};
}

