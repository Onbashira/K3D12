#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Signature/CommandSignature.h"
namespace K3D12{

	//�܂��QD����
	struct ParticleData {
		//�|�W�V���� xyz = Pos w = killedFlag 0 is dead
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
		D3D12_GPU_VIRTUAL_ADDRESS vertexAddres;	//���_�t�F�b�`�͕s�\���H�R���X�^���g�ɂ��ׂ��E�H
		D3D12_GPU_VIRTUAL_ADDRESS cameraAddress;
		D3D12_GPU_VIRTUAL_ADDRESS shaderResourceAddress;
		D3D12_DRAW_ARGUMENTS drawArguments;
	};
	struct ParticleRootConstant {
		//�ő吶����
		unsigned int			_spawnCount;
		//�ő�`��R�}���h��
		unsigned int			_commandCount;

	};

	class ParticleInstanceData
	{
	private:

		//�p�[�e�B�N���p�̃f�B�X�N���v�^�q�[�v
		DescriptorHeap			_particleHeap;
		//�p�[�e�B�N���̐����f�[�^
		UnorderedAccessValue	_particleCreateData;
		//�p�[�e�B�N���̃C���f�b�N�X�p�o�b�t�@
		UnorderedAccessValue	_particleData;
		//�󂫃f�[�^(������ꂽ�f�[�^�j�̊Ǘ� (+Append/Consume)
		UnorderedAccessValue	_reservedIndexData;
		//�R�}���h���X�g�ɓ�����悤�ɍ��`��f�[�^(indirectCommand
		UnorderedAccessValue	_drawArgumentData;
		//�R�}���h����
		CommandSignature		_commandSignature;
		//�R�}���h���߃f�[�^
		std::vector<D3D12_INDIRECT_ARGUMENT_DESC> _drawDescs;
		//

	public:

	private:

	public:

		ParticleInstanceData();

		~ParticleInstanceData();
		
		//
		void Create(unsigned int spawnCountParFrame = 1024, unsigned int commandCountParFrame = 1024);

		void AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties);

		void SetDescriptorHeap(GraphicsCommandList computeCommandList);

	};

}