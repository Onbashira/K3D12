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
		//�C���X�^���V���O����p�����[�^�z��
		std::vector<K3D12::VertexBuffer> _instanceParamater;
		//�C���X�^���V���O���郁�b�V�����
		MeshComponent <T> _instanceMesh;
		std::vector<GameObject> _instanceTransformData;

	public:

	private:

	public:

		InstanceDataManager();
		~InstanceDataManager();
	};

}