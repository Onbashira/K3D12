#pragma once
#include <vector>
#include <map>
#include "../Resource/VertexBuffer.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/MeshRenderer.h"


namespace K3D12 {
	enum class MESH_TYPE {

	};
	template <class T ,class  F>
	class InstanceDataManager
	{

	private:
		//インスタンシングするパラメータ配列
		std::vector<K3D12::VertexBuffer> _instanceParamater;
		//インスタンシングするメッシュ情報
		MeshComponent <T> _instanceMesh;
		std::vector<GameObject> _instanceTransformData;

	public:

	private:

	public:

		InstanceDataManager();
		~InstanceDataManager();
	};

}