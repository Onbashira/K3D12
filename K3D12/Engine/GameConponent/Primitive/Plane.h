#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveComponent.h"

namespace K3D12 {
	class RootSignature;
	class PipelineStateObject;

	class Plane :
		public ModelObject
	{
	private:
	protected:
		PrimitiveComponent _info;
	public:
	private:
	protected:
	public:
		 void Initializer();
		 void RegisterToBundle()override;
		 void Draw()override;
		 void Update()override;
		 void AttachTexture(std::string pathName);
		 void SetUV(float compressionRatio);

		Plane();
		~Plane();
	};
}