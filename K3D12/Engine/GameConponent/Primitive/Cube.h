#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveComponent.h"
#include "../../Model/ModelObject.h"

namespace K3D12 {
	class Cube :
		public ModelObject
	{
	private:
	protected:
		PrimitiveComponent _info;
	public:
	private:
	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle()override;
		//�J�X�^�����_�o�b�t�@���Z�b�g�A�b�v�����邽�߂̊֐�
		virtual void InitializeCustomVBO(void** customVertexDataSrc)override;

	public:

		virtual void Update()override;
		//�`�掞�Ăяo���֐�
		virtual void Draw()override;
		//���f���̃Z�b�g�A�b�v
		void Initializer();
		void AttachTexture(std::string pathName);
		void SetUV(float compressionRatio);

		Cube();
		Cube(Vector3 pos, Vector3 scale);
		~Cube();
	};
}