#pragma once
#include "../Util/NonCopyable.h"

namespace K3D12 {
	class RenderingManager : private NonCopyable
	{
	private:
	public:

	private:
		RenderingManager();
		RenderingManager(const RenderingManager&) {};
		RenderingManager(RenderingManager&&) {};
		RenderingManager& operator =(const RenderingManager&) { return *this; };
		RenderingManager& operator =(RenderingManager&&) { return *this; };

	public:
		void SceneRenderer();
		void SceneRendering();
		~RenderingManager();

	};
}
