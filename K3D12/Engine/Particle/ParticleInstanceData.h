#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "ParticleQuad.h"
namespace K3D12{

	//まず２Dから？
	struct ParticleData {
		//ポジション
		Vector2 pos;
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
		D3D12_GPU_VIRTUAL_ADDRESS gpuAddress;
		D3D12_DRAW_ARGUMENTS drawArguments;

	};


	class ParticleInstanceData
	{
	private:
		
		//パーティクル用のディスクリプタヒープ
		DescriptorHeap			_particleHeap;
		//パーティクルの生データ
		UnorderedAccessValue	_particleRawData;
		//空きデータ(解放されたデータ）の管理
		UnorderedAccessValue	_particleReleasedIndexData;
		//パーティクルのインデックス用バッファ
		UnorderedAccessValue	_particle;
		//コマンドリストに投げるように作る描画データ
		UnorderedAccessValue	_particleKickData;

	public:

	private:

	public:

		ParticleInstanceData();

		~ParticleInstanceData();

	};

}