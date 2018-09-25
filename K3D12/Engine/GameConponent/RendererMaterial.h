#pragma once
#include "../Util/Math.h"
namespace K3D12 {

	struct RendererMaterial
	{
		//�g�U���ˌ� w = alpha
		Vector4 diffuse;
		//���ʔ��ˌ�	w = pow
		Vector4 specular;
		//����
		Vector3 ambient;
		//���Ȕ����F
		Vector3 emissive;
	};

	struct RendererPBRMaterial
	{
		Vector4 albedo;		//�A���x�h
		float reflectivity; //��������
		float roughness; //microsurface�l
	};
}