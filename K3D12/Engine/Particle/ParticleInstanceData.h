#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Signature/CommandSignature.h"
namespace K3D12{

	//まず２Dから
	struct ParticleData {
		//ポジション xyz = Pos w = killedFlag 0 is dead
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


	struct IndirectCommand {
		D3D12_GPU_VIRTUAL_ADDRESS vertexAddres;	//頂点フェッチは不可能か？コンスタントにすべき・？
		D3D12_GPU_VIRTUAL_ADDRESS cameraAddress;
		D3D12_GPU_VIRTUAL_ADDRESS shaderResourceAddress;
		D3D12_DRAW_ARGUMENTS drawArguments;
	};
	struct ParticleRootConstant {
		//最大生成数
		unsigned int			_spawnCount;
		//最大描画コマンド数
		unsigned int			_commandCount;

	};

	class ParticleInstanceData
	{
	private:

		//パーティクル用のディスクリプタヒープ
		DescriptorHeap			_particleHeap;
		//パーティクルの生成データ
		UnorderedAccessValue	_particleCreateData;
		//パーティクルのインデックス用バッファ
		UnorderedAccessValue	_particleData;
		//空きデータ(解放されたデータ）の管理 (+Append/Consume)
		UnorderedAccessValue	_reservedIndexData;
		//コマンドリストに投げるように作る描画データ(indirectCommand
		UnorderedAccessValue	_drawArgumentData;
		//コマンド解釈
		CommandSignature		_commandSignature;
		//コマンド解釈データ
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