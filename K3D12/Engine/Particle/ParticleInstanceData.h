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

	struct ParticleIndirectCommand {
		D3D12_GPU_VIRTUAL_ADDRESS	vertexBufferGPUAddress; //GPU�A�h���X
		unsigned int				vertexBufferSize;//�P���_�T�C�Y�@
		unsigned int				vertexBufferStride;//�P���_�X�g���C�h
		D3D12_GPU_VIRTUAL_ADDRESS	cameraConstantData;//�J�����̃R���X�^���g�f�[�^�ւ̃X�g���C�h
		D3D12_GPU_DESCRIPTOR_HANDLE	shaderResourceGPUAddress;
		D3D12_DRAW_ARGUMENTS		drawArguments;
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
		COMPUTE_SLOT_CONSTANT,
		COMPUTE_SLOT_PARTICLE_CREATE_DATA,
		COMPUTE_SLOT_PARTICLE_DATA,
		COMPUTE_SLOT_DRAW_ARGUMENT_DATA,
		COMPUTE_SLOT_RESERVE_SLOT_CONSUME_DATA,
		COMPUTE_SLOT_RESERVE_SLOT_APPEND_DATA,
		COMPUTE_SLOT_PARTICLE_COMPUTE_SLOT_MAX
	};

	enum PARTICLE_GRAPHICS_SLOT {
		GRAPHICS_SLOT_CONSTANT,
		GRAPHICS_SLOT_PARTICLE_CREATE_DATA,
		GRAPHICS_SLOT_PARTICLE_DATA,
		GRAPHICS_SLOT_DRAW_ARGUMENT_DATA,
		GRAPHICS_SLOT_RESERVE_SLOT_CONSUME_DATA,
		GRAPHICS_SLOT_RESERVE_SLOT_APPEND_DATA,
		GRAPHICS_SLOT_PARTICLE_COMPUTE_SLOT_MAX
	};

	enum PARTICLE_HEAP_DESCRIPTOR_OFFSET {
		OFFSET_PARTICLE_CREATE_DATA,
		OFFSET_PARTICLE_DATA,
		OFFSET_DRAW_ARGUMENT_DATA,
		OFFSET_RESERVE_SLOT_CONSUME_DATA,
		OFFSET_RESERVE_SLOT_APPEND_DATA,
		OFFSET_SPRITE,
		OFFSET_PARTICLE_OFFSET_MAX
	};

	//�̂��ɍ��������c�ݕ`��ɗp����

	//

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
		//�R�}���h���X�g�ɓ�����悤�ɍ��`��f�[�^(ParticleIndirectCommand)
		UnorderedAccessValue	_drawArgumentData;
		//�R�}���h����
		CommandSignature		_commandSignature;
		//�R�}���h���߃f�[�^
		std::vector<D3D12_INDIRECT_ARGUMENT_DESC>	_drawDescs;
		//�C���_�C���N�gDraw���g��Ȃ�	(�e�X�g�p) (��ŊO������ݒ�ł���悤�ɂ���)
		D3D12_VERTEX_BUFFER_VIEW					_vertexBufferView;
		//�p�[�e�B�N���`��R�}���h�������R���e�i	(�e�X�g�p) (��ŊO������ݒ�ł���悤�ɂ���)
		std::vector<ParticleIndirectCommand>		_indirectDrawBuffer;
		//�p�[�e�B�N���������R���e�i	(�e�X�g�p) (��ŊO������ݒ�ł���悤�ɂ���)
		std::vector<ParticleData>					_particleInitBuffer;


	public:

	private:

		void InitHeap();

		void InitView();

		void LoadParticleTexture(std::string path);

	public:

		ParticleInstanceData();

		~ParticleInstanceData();

		void Create(unsigned int spawnCountParFrame = 1024, unsigned int commandCountParFrame = 1024);

		void CreateCommandSignature(std::weak_ptr<K3D12::RootSignature> rs);

		void AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties);

		void SetDescriptorHeap(std::weak_ptr<GraphicsCommandList> computeCommandList);

		void SetVertexBuffer(std::weak_ptr<GraphicsCommandList> graphicsCommandList);

		void InitParticleIndirectCommandBuffer(unsigned int size, unsigned int stride, void* pBuffer);

		void InitParticleInitializeBuffer(unsigned int size, unsigned int stride, void* pBuffer);



		void Discard();

	};

}