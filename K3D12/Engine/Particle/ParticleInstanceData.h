#pragma once
#include <vector>
#include <memory>

#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Signature/CommandSignature.h"

namespace K3D12 {
	class RootSignature;
	//�܂��QD����
	struct ParticleData {
		//�|�W�V���� xyz = Pos w = killedFlag
		Vector4 pos;
		//�x���V�e�B
		Vector2 velocity;
		//�T�C�Y
		float size;
		//�p�x
		float angle;
		//��������
		float liveTime;
	};

	struct IndirectCommand {
		D3D12_GPU_VIRTUAL_ADDRESS cameraAddress;
		D3D12_GPU_VIRTUAL_ADDRESS vertexAddres;
		D3D12_GPU_VIRTUAL_ADDRESS shaderResourceAddress;
		D3D12_DRAW_ARGUMENTS drawArguments;
	};

	struct ParticleRootConstant {
		//�ő吶����
		unsigned int			_spawnCount;
		//�ő�`��R�}���h��
		unsigned int			_commandCount;
		//�p�f�B���O
		unsigned int			_padding01;
		//�p�f�B���O
		unsigned int			_padding02;
	};

	enum PARTICLE_COMPUTE_SLOT {
		CONSTANT,
		PARTICLE_CREATE_DATA,
		PARTICLE_DATA,
		DRAW_ARGUMENT_DATA,
		RESERVE_SLOT_CONSUME_DATA,
		RESERVE_SLOT_APPEND_DATA,
		PARTICLE_COMPUTE_SLOT_MAX
	};

	class ParticleInstanceData
	{
	private:
		//�p�[�e�B�N��32Bits�f�[�^
		ParticleRootConstant    _particleConstants;
		//�p�[�e�B�N���������i�f�o�b�O�p�j
		unsigned int			_liveParticleNum;
		//�p�[�e�B�N���p�̃f�B�X�N���v�^�q�[�v
		DescriptorHeap			_particleHeap;
		//�p�[�e�B�N���̐����f�[�^
		UnorderedAccessValue	_particleCreateData;
		//�p�[�e�B�N���̃C���f�b�N�X�p�o�b�t�@
		UnorderedAccessValue	_particleData;
		//�󂫃f�[�^(������ꂽ�f�[�^�j�̊Ǘ� (+Append/Consume)
		UnorderedAccessValue	_reservedIndexData;
		//�R�}���h���X�g�ɓ�����悤�ɍ��`��f�[�^(indirectCommand)
		UnorderedAccessValue	_drawArgumentData;
		//�R�}���h����
		CommandSignature		_commandSignature;
		//�R�}���h���߃f�[�^
		std::vector<D3D12_INDIRECT_ARGUMENT_DESC> _drawDescs;

	public:

	private:

	public:

		ParticleInstanceData();

		~ParticleInstanceData();

		void Create(unsigned int spawnCountParFrame = 1024, unsigned int commandCountParFrame = 1024);

		void CreateCommandSignature(std::weak_ptr<K3D12::RootSignature> rs);

		void AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties);

		void SetDescriptorHeap(GraphicsCommandList computeCommandList);

		void Discard();

	};

}