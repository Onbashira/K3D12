#pragma once
#include <list>
#include "../../Engine/Util/D3D12Common.h"
#include "../../Engine/Util/Math.h"
#include "../../Engine/Util/Utility.h"
#include "../../Engine/Resource/UploadBuffer.h"
#include "../../Engine/DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {
	class Particle
	{	
	public:

		struct EmitterParam {
			unsigned int maxNum;
			Vector3 position;
			Vector3 rotation;
			Vector3 velocity;
			Vector3 addParam;
			float randamYaw;
			float randamPitch;
			float interval;
			float lifeTime;
			bool isHoming;
		};

	private:

		struct ParticleVertex{
			Vector3 pos;
			float   rad;
			Vector4 color;
		};

		struct VertexPararm {
			Vector3 velocity;
			Vector3 add;
			float lifeTime;
		};

		struct ConstantBuffer {
			Matrix wvp;
			Matrix invView;
		};

		DescriptorHeap  _constantsHeap;
		UploadBuffer<ConstantBuffer> _constantBuffer;;
		UploadBuffer<ParticleVertex> _vertexBuffer;;
		EmitterParam _emmiterParam;
		std::vector<ParticleVertex> _vertices;
		std::list<VertexPararm> _vertexParams;
		unsigned int _vertexBufferStride;
		double _time;

	private:

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() {

			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = _vertexBuffer.GetResource()->GetGPUVirtualAddress();
			vbv.StrideInBytes = _vertexBufferStride;
			vbv.SizeInBytes = _vertices.size() * _vertexBufferStride;

			return vbv;
		}

	public:

		Particle();

		~Particle();

		void Create(EmitterParam param);

		void Run(float deltaTime);

		void Draw();
	};

}