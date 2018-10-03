#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveComponent.h"
#include "../../Model/ModelObject.h"

namespace K3D12 {
	class Capsule :
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
		void RegistBundle();
		void SetPipelineState(PipelineStateObject* pipelineState);
		void SetRootSignature(RootSignature* rootSignature);
		void Draw();
		void Update();
		void LateUpdate();
		Capsule();
		~Capsule();
	};
}