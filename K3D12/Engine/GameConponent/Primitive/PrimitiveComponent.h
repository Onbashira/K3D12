#pragma once
#include <vector>
#include <memory>
#include "../RendererMaterial.h"
#include "../../Resource/ShaderResource.h"
#include "Math.h"


namespace K3D12 {
	class ShaderResource;

	struct PrimitiveComponent
	{

		enum class PRIMITIVE_ROOT_IDNEX {
			CAMERA_PARAMATER_INDEX = 0,
			TRANSFORM_PARAMATER_INDEX,
			MATERIAL_PARAMATER_INDEX,
			SHADER_PARAMATER_INDEX
		};
		RendererMaterial _materialInfo;
		PrimitiveComponent()  {};
		~PrimitiveComponent() {};
	};
}