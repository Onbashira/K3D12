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