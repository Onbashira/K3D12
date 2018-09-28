#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "RenderingPathBase.h"
#include "../Util/ManagerComponentBase.h"
namespace K3D12 {

	//シーン内の1パス単位のクラス
	class RootRenderingPath                                                             
	{
	private:
		bool													_isEnable;
		K3D12::UnorderedManagerComponentBase<RenderingPathBase> _renderingPathMap;
		CommandQueue&											_excutionQueue;
	public:

	private:

	public:
		void Rendering();
		void ChangeExcutionQueue(CommandQueue& excutionQueue);
		std::shared_ptr<RenderingPathBase> AddRenderingPath(std::string name, bool isEnable = true);
		void EraseRenderingPath(std::string name);
		std::weak_ptr<RenderingPathBase> GetRenderingPath(std::string name);
		bool IsLoaded(std::string name);
		void DiscardRenderingPath();

		void Enable();
		void Disable();
		bool IsEnable();

		RootRenderingPath(CommandQueue& excutionQueueRef);
		RootRenderingPath(const RootRenderingPath& other);
		RootRenderingPath(RootRenderingPath&& other);
		RootRenderingPath& operator =(const RootRenderingPath& other);
		RootRenderingPath& operator =(RootRenderingPath&& other);
		~RootRenderingPath();
	};

}