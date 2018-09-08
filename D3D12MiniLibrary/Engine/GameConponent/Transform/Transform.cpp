#include <utility>
#include "Transform.h"
#include "../../Util/Math.h"



///
///
///
///DirectX�̃��[���h�ϊ��s��v�Z�͒ʏ�̋t�̏����ōs��
///�@���e���_�@���@��]�s��@���@�ړ����@�݂����Ȋ�����
///�t��HLSL�͒ʏ�̌v�Z���l�̏����ōs��
///	 ���ړ��@��	��]�s��@���@�e���_��
///
//���؂P�F���[�J���̃��f���ɍs����|����
//���f���͎����̋�ԁi���[�J����ԁj�ɑ��݂���
//���f���̃|���S�����\������u���_���W�v�̒l�����[�J����Ԃ̍��W�n�ŕ\������Ă���͂��Ȃ̂�
//���̃��f���ɑ΂��č��W�ϊ��s��M��K�p����ƁA���̃��f���̒��_���W�͉��炩�̕ʂ̈ʒu�Ɉړ�����B
///
//DirectX�̍s��`���ł́A�E�։E�ւƍ��W�ϊ��s����|���Z���Ă����ƁA
//�����͑S�ĕϊ���ɍl���Ă��鐢�E�̍��W�n����Ƃ��鎖�ɂȂ�
///
//�Ȃ̂ŁA���̂܂܂��Ɛ��E����ɂȂ�̂ŁA���[�J����ɂȂ�悤�ɕύX����
///
///

K3D12::Transform::Transform() :
	_pos(), _rotation(Quaternion::CreateIdentity())
{

}


K3D12::Transform::~Transform()
{
	_pos = Vector3::zero;
	_rotation = Quaternion::CreateIdentity();


}


void K3D12::Transform::SetPos(const Vector3  pos)
{
	_pos = pos;
}

void K3D12::Transform::SetRotation(const Quaternion rotation)
{
	_rotation = rotation;
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::SetEulerAngles(const Vector3 eulerAugles)
{
	_eulerAngles = eulerAugles;
	_rotation = std::move(Quaternion::CreateFromEulerAngles(eulerAugles));
}

Vector3 K3D12::Transform::GetPos()
{
	return _pos;
}

Quaternion  K3D12::Transform::GetRotation()
{
	return	_rotation;
}

Vector3 K3D12::Transform::GetEulerAngles()
{
	return _rotation.EulerAngles();
}

void K3D12::Transform::Translate(const Vector3 & velocity)
{
	_pos += velocity;
}

void K3D12::Transform::Move(const Vector3 & velocity)
{
	_pos += Vector3::Rotate(velocity, _rotation);

}

void K3D12::Transform::Rotation(const Quaternion & rot)
{
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::RotationLocalWorld(const Quaternion & rot)
{
	//���[�J�����ŉ�]��������
	auto axis = GetLocalAxis();


}

void K3D12::Transform::RotationLocalWorld(const Vector3 & eulerAngles)
{
	//���[�J�����ŉ�]��������
	auto axis = GetLocalAxis();
	auto uRot = std::move(Quaternion::CreateFromAxisAngles(axis.u, DegToRad(eulerAngles.x)));
	auto vRot = std::move(Quaternion::CreateFromAxisAngles(axis.v, DegToRad(eulerAngles.y)));
	auto wRot = std::move(Quaternion::CreateFromAxisAngles(axis.w, DegToRad(eulerAngles.z)));

}

void K3D12::Transform::RotationEulerAngles(const Vector3 & eulerAngles)
{
	_eulerAngles += eulerAngles; 
	_rotation = std::move(Quaternion::CreateFromEulerAngles(_eulerAngles));
}

void K3D12::Transform::RotationAxisAngles(const Vector3 & axis, float rad)
{
	_rotation *= std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::RotateAround(const Vector3 & point, const Quaternion & rot)
{
	auto v = std::move(_pos - point);
	auto vv = std::move(Vector3::Rotate(v, rot));
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
	_pos = std::move(vv + (point));
}

void K3D12::Transform::RotateAround(const Vector3 & point, const Vector3 & axis, float rad)
{
	auto q = std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	RotateAround(point, q);
}


void K3D12::Transform::LookAt(const Vector3 & target, const Vector3 & up)
{
	//�^�[�Q�b�g�����[�J����ԂɎ����Ă���
	Vector3 localTargetPos = target;
	localTargetPos = Vector3::Transform(localTargetPos, GetView());
	_rotation = std::move(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix((Matrix::CreateLookAt(Vector3::zero, localTargetPos, up)))).Normalize());
	_eulerAngles = _rotation.EulerAngles();
}

OrthonormalBasis K3D12::Transform::GetLocalAxis()
{
	OrthonormalBasis orthonomal;
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));
	orthonomal.u = Vector3(mat.axisX).Normalize();
	orthonomal.v = Vector3(mat.axisY).Normalize();
	orthonomal.w = Vector3(mat.axisZ).Normalize();
	return orthonomal;
}

Matrix K3D12::Transform::GetView()
{
	//��]�s��
	auto mat = Matrix::CreateFromQuaternion(_rotation);

	mat._41 = _pos.x;
	mat._42 = _pos.y;
	mat._43 = _pos.z;
	mat._44 = 1.0f;

	mat = Matrix::Invert(mat);
	//mat.EpsilonCheck();

	return mat;
}

