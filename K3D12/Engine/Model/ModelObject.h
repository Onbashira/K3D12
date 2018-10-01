#pragma once
#include "../GameConponent/GameObject/GameObject.h"
#include "../DrawnableComponent/DrawableComponent.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/MeshRenderer.h"
namespace K3D12 {
	template <class T>
	//�x�[�X�̃��f���I�u�W�F�N�g�@��{���b�V�����A�`�悷�邽�߂̃q�[�v�A�`�悷�邽�߂̃p�C�v���C�����A�Q�[���I�u�W�F�N�g����������
	class ModelObject : public GameObject ,public DrawableComponent,public MeshComponent<T>,public MeshRenderer
	{
	private:

	public:

	private:

	public:
		ModelObject();
		virtual ~ModelObject();
	};
}

