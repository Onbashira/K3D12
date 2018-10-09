#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveObject.h"

namespace K3D12 {
	class Plane :
		public PrimitiveObject
	{
	private:
	protected:
	public:
	private:
	protected:
		void RegisterToBundle()override;
	public:
		 void Initializer();
		 void Draw()override;
		 void Update()override;
		 void AttachTexture(std::string pathName);
		 void SetUV(float compressionRatio);
		Plane();
		~Plane();
	};
}