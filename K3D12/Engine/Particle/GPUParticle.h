#pragma once
#include <memory>
#include <vector>
#include "../../Engine/Util/Math.h"
#include "../../Engine/Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Signature/CommandSignature.h"
#include "../Resource/UploadBuffer.h"
#include "../Resource/StructuredBuffer.h"
#include "ParticleCSStruct.h"

namespace K3D12{

	class GPUParticle {

	public:

	private:

		static unsigned int INSTANCE_MAX;
		static unsigned int SPAWN_MAX;

		//�V�[���p�̃R���X�^���g
		struct SceneConstantBuffer {
			float gameTime;
			float deltaTime;
			unsigned int spawnerCount;
			float rad;
		};

		struct DrawArgs {
			//4096�o�C�g�ڂ�InstanceCount������悤�ɒ���
			unsigned char pad[D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 4];
			unsigned int VertexCountPerInstance;
			unsigned int InstanceCount;
			unsigned int StartVertexLocation;
			unsigned int StartInstanceLocation;
			unsigned int pad2[2];
		};

		struct ParticleData {
			bool isActive;				// �L���t���O
			Vector3 position;	// ���W
			Vector3 velocity;	// �����x
			Vector4 color;		// �F
			float duration;				// ��������
			float scale;				// �T�C�Y
		};

		//�h���[�I�[�O�����g
		struct IndirectCommand {
			D3D12_DRAW_ARGUMENTS drawArgment;
		};

		StructuredBuffer _reservedSlotsBuffer;

		StructuredBuffer _instancePropatiesBuffer;

		StructuredBuffer _instanceDrawBuffer;

		StructuredBuffer _instanceCountBuffer;

		StructuredBuffer _drawArgBuffer;

		StructuredBuffer _drawArgCopyBuffer;

		DescriptorHeap   _initDescriptorHeap;

		DescriptorHeap   _descriptorHeap;

		DescriptorHeap   _drawDescriptorHeap;

		CommandSignature _commandSignature;

		UploadBuffer<SpawnData> _spawnData;

		UploadBuffer<SceneConstantBuffer> _sceneConstantBuffer;

		UploadBuffer<Matrix> _wvpMatBuffer;

		int PARTICLE_MAX;

		int EMIT_NUM;

		int _spawanDataCount;

		float _deltaTime;

		float _gameTime;


	private:

		void CreateBuffers();

		void CreateRootSignature();

		void CreateDescriptorHeap();

		void CreatePipelineState();

	public:

		GPUParticle();

		~GPUParticle();

		void Create(int particleMax, int emitNum);

		void Run(float deltaTime);

		void Draw();

		void Spawn(int num, Vector3 pos, float speedMag = 10.0f, float lengthMag = 4.0f, float reductionRate = 0.9375f);

	};
}

