#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"
#include "ParticleQuad.h"
namespace K3D12{

	//まず２Dから？
	struct ParticleData {
		//ポジs田
		Vector2 pos;
		//UV座標
		Vector2 texcorrd;
		//ベロシティ
		Vector2 velocity;
		//角度
		float angle;
		//生存時間
		float liveTime;
	};

	class ParticleInstanceData
	{
	private:
		//パーティクルのポリゴンデータ
		ParticleQuad			_quad;
		//パーティクルの生データ
		UnorderedAccessValue	_particleRawData;
		//空きデータ(解放されたデータ）の管理
		UnorderedAccessValue	_particleReleasedIndexData;
		//死んだパーティクルのインデックス用バッファ
		UnorderedAccessValue	_particle;

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