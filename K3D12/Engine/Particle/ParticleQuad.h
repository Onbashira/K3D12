#pragma once
#include <string>
#include "ParticleInstanceData.h"
#include "../Model/ModelObject.h"
#include "../DescriptorHeap/DescriptorHeap.h"
namespace K3D12 {
	//パーティクルのクアッド
	class ParticleQuad : public ModelObject
	{
	private:		
	public:

	private:

		void Initialize();

		void RegisterToBundle()override;

	public:
	
		ParticleQuad();
		
		~ParticleQuad();

		void Update();

		void Draw();

		void InstanceDraw();

		void IndirectDraw();

		void Discard();

		void AttachTexture(std::string path);

	};
}
