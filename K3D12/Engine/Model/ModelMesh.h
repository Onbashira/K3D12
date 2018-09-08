#pragma once

#include <memory>
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../GameConponent/GameObject/GameObject.h"





namespace K3D12 {

	class IndexBuffer;
	class VertexBuffer;
	class ConstantBuffer;
	class ShaderResource;

	//���b�V���ɕK�v�ȏ����������N���X
	class ModelMesh : public K3D12::GameObject,
		public K3D12::DrawableComponent
	{
	private:

	protected:
		std::unique_ptr<K3D12::IndexBuffer>	_indexBuffer;	//���f���̒��_�C���f�b�N�X
		std::unique_ptr<K3D12::VertexBuffer>	_vertexBuffer;	//���f���̒��_�o�b�t�@
		std::unique_ptr<K3D12::DescriptorHeap> _heap;			//�f�X�N���v�^�q�[�v�@�i�ό`��� + �}�e���A���� + �e�N�X�`����)

		unsigned int					_transformStartPoint;	//�ό`���f�X�N���v�^�̊J�n�ʒu		(maybe 0)
		unsigned int					_materialStartPoint;		//�}�e���A���f�X�N���v�^�̊J�n�ʒu	(maybe 0+1 ~ x)
		unsigned int					_textureStartPoint;		//�e�N�X�`���f�X�N���v�^�̊J�n�ʒu		(maybe x+1 ~ y)

		K3D12::ConstantBuffer					_materialBuffer;	//�}�e���A�����̃o�b�t�@
		std::vector<std::weak_ptr<K3D12::ShaderResource>> _textureResource; //�e�N�X�`�����\�[�X�ւ̎Q��
	public:

	private:

	protected:

	public:

		void BindingVertexBuffer(K3D12::GraphicsCommandList* list);
		void BindingIndexBuffer(K3D12::GraphicsCommandList* list);
		void BindingDescriptorHeaps(K3D12::GraphicsCommandList* list);
		void SetTextureCount(unsigned int textureCount = 0);
		void SetMaterialCount(unsigned int materialCount = 1);
		ModelMesh();
		~ModelMesh();
	};
}

