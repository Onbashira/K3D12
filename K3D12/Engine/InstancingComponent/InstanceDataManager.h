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
		//�C���X�^���V���O����p�����[�^�z��
		std::vector<K3D12::VertexBuffer> _instanceParamater;
		//�C���X�^���V���O���郁�b�V�����
		
	public:

	private:

	public:

		InstanceDataManager();
		~InstanceDataManager();
	};

}