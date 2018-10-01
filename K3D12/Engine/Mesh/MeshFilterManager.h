#pragma once
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "../Util/NonCopyable.h"
#include "../Util/ManagerComponentBase.h"

namespace K3D12 {
	class MeshFilterManager : private NonCopyable
	{	
	private:
		UnorderedManagerComponentBase<MeshComponent<>> _manager;
	public:
		
	private:
		

		MeshFilterManager() {};
		~MeshFilterManager() {};
	public:

	};
}