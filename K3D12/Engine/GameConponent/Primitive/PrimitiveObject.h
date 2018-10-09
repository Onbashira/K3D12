#pragma once
#include "../../Model/ModelObject.h"
#include "../Primitive/PrimitiveComponent.h"
namespace K3D12 {

	class PrimitiveObject :public  ModelObject
	{
	private:
	protected:
		PrimitiveComponent _info;
	public:

	private:

	protected:
		virtual void RegisterToBundle()override = 0;
	public:
		virtual void FixedUpdate();
		virtual void Update()override = 0;
		virtual void Draw()override = 0;
		PrimitiveObject();
		virtual ~PrimitiveObject();
	};
}

