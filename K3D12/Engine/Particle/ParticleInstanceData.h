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
	//まず２Dから
	struct ParticleData {
		//ポジション xyz = Pos w = killedFlag
		Vector4 pos;
		//ベロシティ
		Vector2 velocity;
		//サイズ
		float size;
		//角度
		float angle;
		//生存時間
		float liveTime;
	};

	struct ParticleIndirectCommand {
		D3D12_GPU_VIRTUAL_ADDRESS	vertexBufferGPUAddress; //GPUアドレス
		unsigned int				vertexBufferSize;//１頂点サイズ　
		unsigned int				vertexBufferStride;//１頂点ストライド
		D3D12_GPU_VIRTUAL_ADDRESS	cameraConstantData;//カメラのコンスタントデータへのストライド
		D3D12_GPU_DESCRIPTOR_HANDLE	shaderResourceGPUAddress;
		D3D12_DRAW_ARGUMENTS		drawArguments;
	};

	struct ParticleRootConstant {
		//最大生成数
		unsigned int			_spawnCount;
		//最大描画コマンド数
		unsigned int			_commandCount;
		//パディング
		unsigned int			_padding01;
		//パディング
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

	//のちに高速水球歪み描画に用いる

	//

	class ParticleInstanceData
	{
	private:
		//パーティクル32Bitsデータ
		ParticleRootConstant    _particleConstants;
		//パーティクル生存数（デバッグ用）
		unsigned int			_liveParticleNum;
		//パーティクル用のディスクリプタヒープ
		DescriptorHeap			_particleHeap;
		//パーティクルの生成データ
		UnorderedAccessValue	_particleCreateData;
		//パーティクルのインデックス用バッファ
		UnorderedAccessValue	_particleData;
		//空きデータ(解放されたデータ）の管理 (+Append/Consume)
		UnorderedAccessValue	_reservedIndexData;
		//コマンドリストに投げるように作る描画データ(ParticleIndirectCommand)
		UnorderedAccessValue	_drawArgumentData;
		//コマンド解釈
		CommandSignature		_commandSignature;
		//コマンド解釈データ
		std::vector<D3D12_INDIRECT_ARGUMENT_DESC>	_drawDescs;
		//インダイレクトDrawを使わない	(テスト用) (後で外部から設定できるようにする)
		D3D12_VERTEX_BUFFER_VIEW					_vertexBufferView;
		//パーティクル描画コマンド初期化コンテナ	(テスト用) (後で外部から設定できるようにする)
		std::vector<ParticleIndirectCommand>		_indirectDrawBuffer;
		//パーティクル初期化コンテナ	(テスト用) (後で外部から設定できるようにする)
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