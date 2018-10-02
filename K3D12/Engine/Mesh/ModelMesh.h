#pragma once

#include <memory>
#include <vector>
#include <memory>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "IMesh.h"
#include "MeshHeap.h"




namespace K3D12 {

	//���f���̃��b�V���ɕK�v�ȏ����������N���X(�n�u
	class ModelMesh : public DrawableComponent, public IMesh
	{
	private:
		MeshHeap _meshHeap;
	protected:
	public:

	private:

	protected:

	public:
		MeshHeap& GetMeshHeap();
		
		void DiscardMeshHeap();
		ModelMesh();
		virtual ~ModelMesh();
	};
};

