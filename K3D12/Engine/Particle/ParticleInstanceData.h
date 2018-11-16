#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "ParticleQuad.h"
namespace K3D12{

	//�܂��QD����H
	struct ParticleData {
		//�|�W�V����
		Vector2 pos;
		//�x���V�e�B
		Vector2 velocity;
		//�T�C�Y
		float size;
		//�p�x
		float angle;
		//��������
		float liveTime;
	};



	class ParticleInstanceData
	{
	private:
		
		//�p�[�e�B�N���p�̃f�B�X�N���v�^�q�[�v
		DescriptorHeap			_particleHeap;
		//�p�[�e�B�N���̐��f�[�^
		UnorderedAccessValue	_particleRawData;
		//�󂫃f�[�^(������ꂽ�f�[�^�j�̊Ǘ�
		UnorderedAccessValue	_particleReleasedIndexData;
		//�p�[�e�B�N���̃C���f�b�N�X�p�o�b�t�@
		UnorderedAccessValue	_particle;
		//�R�}���h���X�g�ɓ�����悤�ɍ��`��f�[�^
		UnorderedAccessValue	_particleKickData;

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