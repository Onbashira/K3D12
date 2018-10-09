#pragma once
#include "PrimitiveComponent.h"
#include  "PrimitiveObject.h"
namespace K3D12 {
	class Cube : public  PrimitiveObject
	{
	private:
	protected:
	public:
	private:
	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle()override;
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