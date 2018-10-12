#pragma once
#include "../Util/NonCopyable.h"
#include "../Util/ManagerComponentBase.h"
namespace K3D12 {
	class IAudioSource;
	class AudioSourceManager : private NonCopyable
	{
	private:
		UnorderedManagerComponentBase<IAudioSource> _resourceMap;
	public:

	private:
		
		AudioSourceManager();
		
		AudioSourceManager(const AudioSourceManager&value) {};
		
		AudioSourceManager(AudioSourceManager&&value) {};
		
		void operator = (const AudioSourceManager& value) {};
		
		void operator = (AudioSourceManager&&value) {};

	public:

		~AudioSourceManager();

		void SetResource(std::shared_ptr<IAudioSource> source);
		
		void GetResource(std::string resourceName);
		
		void EraseResource(std::string name);
		
		bool IsLoaded(std::string resourceName);
		
		void DiscardManager();
	};
}

