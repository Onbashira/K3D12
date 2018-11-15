#pragma once

#include <memory>
#include <vector>
#include <memory>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"
#include "MeshBuffer.h"
#include "MeshHeap.h"




namespace K3D12 {

	//���f���̃��b�V���`��ɕK�v�ȃN���X
	class ModelMesh : public DrawableComponent
	{
	private:
		MeshHeap _meshHeap;

		MeshBuffer _meshBuffer;
	protected:

	public:

	private:

	protected:

		virtual void BindVertexBufferToBundle();

		virtual void BindIndexBufferToBundle();
		//�o���h�����X�g�Ɍp����Ńh���[�R�[���o�b�`����Ƃ�����
		virtual void RegisterToBundle()override = 0;

	public:

		//�`�掞�Ăяo���֐��̍쐬������
		virtual void Draw()override = 0;

		MeshHeap& GetMeshHeap();

		MeshBuffer& GetMeshBuffer();

		void BindDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list);

		void BindDescriptorHeaps(K3D12::GraphicsCommandList & list);

		void DiscardMeshHeap();

		void DiscardMeshBuffer();

		ModelMesh();
		virtual ~ModelMesh();
	};
};

