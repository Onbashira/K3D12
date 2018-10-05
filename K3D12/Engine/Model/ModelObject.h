#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../Mesh/ModelMesh.h"
namespace K3D12 {
	//�x�[�X�̃��f���I�u�W�F�N�g�@��{���b�V�����A�`�悷�邽�߂̃q�[�v�A�`�悷�邽�߂̃p�C�v���C�����A�Q�[���I�u�W�F�N�g����������
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:

	private:

	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle()override = 0;
		//�o���h���Ƀo�C���h���钸�_�o�b�t�@
		virtual void BindVertexBufferToBundle()override;
		//�o���h���Ƀo�C���h����C���f�b�N�X�o�b�t�@
		virtual void BindIndexBufferToBundle()override;

	public:
		//���f���̃A�b�v�f�[�g
		virtual void Update()override = 0;
		//�`�掞�Ăяo���֐��̍쐬������
		virtual void Draw()override = 0;
		//�f�t�H���g�Ŏg��VBO�̃Z�b�g�A�b�v
		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData);
		//�f�t�H���g�Ŏg��IBO�̃Z�b�g�A�b�v
		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData);
		//���[�U�[�w���VBO�̃Z�b�g�A�b�v
		virtual void InitializeCustomVBO(void** customVertexDataSrc)override = 0;

		void DiscardModelObject();
		ModelObject();
		virtual ~ModelObject();
	};
}

