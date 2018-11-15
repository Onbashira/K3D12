#pragma once
#include <vector>
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/UnorderedAccessValue.h"
#include "../Util/Math.h"

namespace K3D12{

	
	struct ParticleData {
		Vector3 pos;

	};

	class ParticleInstanceData
	{
	private:
		UnorderedAccessValue	_particleRawData;
		UnorderedAccessValue	_particleDeadIndexData;
		UnorderedAccessValue	_particleAliveIndexData;
		unsigned int			_deadParticleNum;
		unsigned int			_alivedParticleNum;
	public:

	private:

	public:

		ParticleInstanceData();

		~ParticleInstanceData();

	};

	class ParticleInstanceDataEx
	{
	private:
		UnorderedAccessValue	_particleRawData;
		UnorderedAccessValue	_particleDeadIndexData;
		UnorderedAccessValue	_particleAliveIndexData;
		unsigned int			_deadParticleNum;
		unsigned int			_alivedParticleNum;

	public:

	private:

	public:

		ParticleInstanceDataEx();

		~ParticleInstanceDataEx();

	};

}