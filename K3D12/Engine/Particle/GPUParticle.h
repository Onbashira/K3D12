#pragma once
#include <memory>
#include <vector>
#include "../../Engine/Util/Math.h"
#include "../../Engine/Util/D3D12Common.h"
#include "../Resource/Resource.h"
#include "../Resource/StructuredBuffer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../../Engine//Signature/CommandSignature.h"
#include "../Resource/UploadBuffer.h"
#include "ParticleCSStruct.h"
namespace K3D12 {
#define THREAD_NUM_X 16
	class GPUParticle
	{
	private:
		inline static unsigned int INSTANCE_MAX = 1024 * 2048;
		inline static unsigned int SPAWN_MAX = 256 * THREAD_NUM_X;

		//シーン用のコンスタント
		struct SceneConstantBuffer {
			float gameTime;
			float deltaTime;
			unsigned int spawnerCount;
			float rad;
		};
		struct DrawArgs {
			//4096バイト目にInstanceCountが来るように調整
			unsigned char pad[D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - sizeof(UINT)];
			unsigned int VertexCountPerInstance;
			unsigned int InstanceCount;
			unsigned int StartVertexLocation;
			unsigned int StartInstanceLocation;
			unsigned int pad2[2];
		};

		struct ParticleData {
			bool isActive;				// 有効フラグ
			Vector3 position;	// 座標
			Vector3 velocity;	// 加速度
			Vector4 color;		// 色
			float duration;				// 生存時間
			float scale;				// サイズ
		};

		//ドローオーグメント
		struct IndirectCommand {
			D3D12_DRAW_ARGUMENTS drawArgment;
		};

		StructuredBuffer _reservedSlotsBuffer;

		StructuredBuffer _InstancePropatiesBuffer;

		StructuredBuffer _InstanceDrawBuffer;

		StructuredBuffer _InstanceCountBuffer;

		StructuredBuffer _DrawArgBuffer;

		StructuredBuffer _DrawArgCopyBuffer;

		StructuredBuffer _DrawArgBuffer;

		DescriptorHeap   _DescriptorHeap;

		DescriptorHeap   _DrawDescriptorHeap;

		CommandSignature _commandSignature;

		UploadBuffer<SpawnData> _spawnData;

		UploadBuffer<SceneConstantBuffer> _sceneConstantBuffer;

		UploadBuffer<Matrix>			_wvpMatBuffer;

		int PARTICLE_MAX;

		int EMIT_NUM;

		int _spawanDataCount;

		float _deltaTime;

		float _gameTime;
		
	public:

	private:

		void CreateBuffers();

		void CreateRootSignature();

		void CreateDescriptorHeap();

		void CreatePipelineState();

	public:

		GPUParticle();

		~GPUParticle();

		void Create(int particleMax, int emitNum);

		void Run();

		void Draw();

		void Spawn(int num,Vector3 pos ,float speedMag = 10.0f,float lengthMag = 4.0f ,float reductionRate = 0.9375f );

	};
}

