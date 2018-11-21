#pragma once
#include <vector>
#include <memory>

#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Resource/StructuredBuffer.h"

#include "../Util/Math.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Signature/CommandSignature.h"

namespace K3D12 {
	class RootSignature;

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

	class ParticleInstanceData
	{
	private:

	public:

	private:

		void InitHeap();

		void InitView();

		void LoadParticleTexture(std::string path);

	public:

		ParticleInstanceData();

		~ParticleInstanceData();
	};

}