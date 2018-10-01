#pragma once
#include <memory>
#include <vector>
#include "../Resource/ConstantBuffer.h"
namespace K3D12 {
	class ShaderResource;
	class GraphicsCommandList;
	//Mesh�������_�����O���邽�߂̃N���X�B��ɃV�F�[�_�[�ɓ����邽�߂̃q�[�v���Ǘ�����
	class MeshRenderer
	{
	private:
	public:

	protected:
		std::unique_ptr<K3D12::DescriptorHeap>				_heap;					//�f�X�N���v�^�q�[�v�@					(�ό`��� + �}�e���A���� + �e�N�X�`����)�@�J�����̏��͕ʂœ�����
		unsigned int										_transformStartPoint;	//�ό`���f�X�N���v�^�̊J�n�ʒu		(maybe 0)
		unsigned int										_materialStartPoint;	//�}�e���A���f�X�N���v�^�̊J�n�ʒu		(maybe 0+1 ~ x)
		unsigned int										_textureStartPoint;		//�e�N�X�`���f�X�N���v�^�̊J�n�ʒu		(maybe x+1 ~ y)

		K3D12::ConstantBuffer								_materialBuffer;		//�}�e���A�����̃o�b�t�@ �@�@
		std::vector<std::weak_ptr<K3D12::ShaderResource>>	_textureResource;		//�e�N�X�`�����\�[�X�ւ̎Q��	
	private:

	public:
		void BindingDescriptorHeaps(std::weak_ptr<K3D12::GraphicsCommandList> list);
		void SetHeapOffsetTransformStartIndex(unsigned int startIndex = 0);
		void SetHeapOffsetTextureStartIndex(unsigned int startIndex = 1);
		void SetHeapOffsetMaterialStartIndex(unsigned int startIndex = 2);

		ConstantBuffer& GetMaterialBufffer();
		std::vector<std::weak_ptr<K3D12::ShaderResource>>& GetTexturesRef();

		MeshRenderer& AddTextureRef(std::weak_ptr<K3D12::ShaderResource> textureRef);

		MeshRenderer();
		virtual ~MeshRenderer();
	};
}

