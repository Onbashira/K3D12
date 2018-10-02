#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../Mesh/ModelMesh.h"
namespace K3D12 {
	//�x�[�X�̃��f���I�u�W�F�N�g�@��{���b�V�����A�`�悷�邽�߂̃q�[�v�A�`�悷�邽�߂̃p�C�v���C�����A�Q�[���I�u�W�F�N�g����������
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:

	private:

	public:
		//���f���̃A�b�v�f�[�g
		virtual void Update() = 0;
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle() = 0;
		//�`�掞�Ăяo���֐��̍쐬������
		virtual void DrawModel() = 0;
		//�f�t�H���g�Ŏg��VBO�̃Z�b�g�A�b�v
		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData) = 0;
		virtual void InitializeCustomVBO(void** customVertexDataSrc) = 0;
		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData) = 0;
		virtual void BindingVertexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;
		virtual void BindingIndexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;

		void DiscardModelObject();
		ModelObject();
		virtual ~ModelObject();
	};
}

