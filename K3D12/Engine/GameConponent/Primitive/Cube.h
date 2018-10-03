#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveComponent.h"
#include "../../Resource/UnorderedAccessValue.h"
#include "../../Model/ModelObject.h"

namespace K3D12 {
	class Cube :
		public ModelObject
	{
	private:
	protected:
		PrimitiveComponent _info;
		std::vector<float>   buffer;
	public:
	private:
	protected:
	public:
		void Initializer();
		void RegistBundle();
		void SetPipelineState(PipelineStateObject* pipelineState);
		void SetRootSignature(RootSignature* rootSignature);
		void Draw();
		void Update();
		void LateUpdate();
		void AttachTexture(std::string pathName);
		void SetUV(float compressionRatio);

		Cube();
		Cube(Vector3 pos, Vector3 scale);
		~Cube();
	};
}