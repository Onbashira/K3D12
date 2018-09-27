#pragma once
#include <vector>
#include <map>
#include "../Resource/VertexBuffer.h"


namespace K3D12 {
	enum class MESH_TYPE {

	};

	class InstanceDataManager
	{

	private:
		//インスタンシングするパラメータ配列
		std::vector<K3D12::VertexBuffer> _instanceParamater;
		//インスタンシングするメッシュ情報
		
	public:

	private:

	public:

		InstanceDataManager();
		~InstanceDataManager();
	};

}