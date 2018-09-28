#pragma once
#include <memory>
#include "../Transform/Transform.h"
#include "../../Resource/ConstantBuffer.h"
#include "../../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {

	class GraphicsCommandList;

	class GameObject
	{
	private:

	protected:
		Transform		_transform;
		Vector3			_scale;
		unsigned int	_initializeSize;
		bool			_isEnable;
		ConstantBuffer	_transformBuffer;
	public:

	private:

	protected:

		//�h���N���X�Ńg�����X�t�H�[���̃o�b�t�@�[�ւ̏������ݕ����`���Ăق���
		virtual void UpdateTransformBuffer();
		//�o�b�t�@���\�[�X��Size����1�����o�b�t�@�Ƃ��ė̈���m�ۂ���G
		virtual void InitalizeTransformBuffer(size_t size);
		//�V�F�[�_�[�Ƀo�C���f�B���O
		virtual void SetTransform(int rootParamaterIndex, std::shared_ptr<GraphicsCommandList> list);
	public:

		virtual void Update() = 0;

		void SetPos(const Vector3 pos);

		void SetRotation(const Quaternion rotation);

		void SetEulerAngles(const Vector3 euler);

		void SetScale(const Vector3 scale);

		Vector3 GetPos();

		Quaternion GetRotation();

		Vector3 GetEulerAngles();

		Vector3 GetScale();

		void Enable();

		void Disable();

		bool IsEnable();

		//@fn				�I�u�W�F�N�g�̈ړ��i���[���h����j
		//@param			(velocity) �ω���	
		void				Translate(const Vector3& velocity);

		//@fn				�I�u�W�F�N�g�̈ړ��i���[�J������j
		//@param			(velocity) �ω���	
		void				Move(const Vector3& velocity);

		//@fn				���_���S��](�l�����̐ςŌv�Z)
		//@param			(rot) �ω���	
		void				Rotation(const Quaternion& rot);

		//@fn�I�C���[�p�Ō��_���S��]
		//@param			(eulerAngles) ��Η�	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		//@fn���Ɖ�]�ʂŉ�]
		//@param			(axis)	��
		//@param			(rad)	�ʓx
		void				RotationAxisAngles(const Vector3& axis, float rad);

		//@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		//@param			(point)	���W
		//@param			(rot)	�ω���
		void				RotateAround(const Vector3& point, const  Quaternion& rot);

		//@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		//@param			(point)	���W
		//@param			(axis)	��
		//@param			(rad)	�ʓx
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		//@fn�I�u�W�F�N�g�̃��[�J��Forward�����|�C���g������悤�ɉ�]
		void				LookAt(const Vector3& point, const Vector3& up);

		//@fn���[���h���W�Ō������[�J�����̎擾
		OrthonormalBasis	GetLocalAxis();

		//@fn���̃I�u�W�F�N�g�̃r���[�s����擾
		Matrix				GetView();

		//@fn���̃I�u�W�F�N�g��SRT�s����擾
		Matrix				GetSRTMatrix();

		GameObject();
		GameObject(Transform transform);

		virtual ~GameObject();
	};

}
