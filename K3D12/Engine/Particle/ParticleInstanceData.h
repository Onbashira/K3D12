#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "ParticleQuad.h"
namespace K3D12{

	//�܂��QD����H
	struct ParticleData {
		//�|�Ws�c
		Vector2 pos;
		//UV���W
		Vector2 texcorrd;
		//�x���V�e�B
		Vector2 velocity;
		//�p�x
		float angle;
		//��������
		float liveTime;
	};

	class ParticleInstanceData
	{
	private:
		//�p�[�e�B�N���̃|���S���f�[�^
		ParticleQuad			_quad;
		//�p�[�e�B�N���̐��f�[�^
		UnorderedAccessValue	_particleRawData;
		//�󂫃f�[�^(������ꂽ�f�[�^�j�̊Ǘ�
		UnorderedAccessValue	_particleReleasedIndexData;
		//���񂾃p�[�e�B�N���̃C���f�b�N�X�p�o�b�t�@
		UnorderedAccessValue	_particle;

	public:

	private:

	public:

		ParticleInstanceData();

		~ParticleInstanceData();

	};

	//class ParticleInstanceDataEx
	//{
	//private:
	//	UnorderedAccessValue	_particleRawData;
	//	UnorderedAccessValue	_particleDeadIndexData;
	//	UnorderedAccessValue	_particleAliveIndexData;
	//	unsigned int			_deadParticleNum;
	//	unsigned int			_alivedParticleNum;

	//public:

	//private:

	//public:

	//	ParticleInstanceDataEx();

	//	~ParticleInstanceDataEx();

	//};

}