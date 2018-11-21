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
		//�ő吶����
		unsigned int			_spawnCount;
		//�ő�`��R�}���h��
		unsigned int			_commandCount;
		//�p�f�B���O
		unsigned int			_padding01;
		//�p�f�B���O
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