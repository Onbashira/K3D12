#pragma once
#include <memory>
#include <vector>
#include "Math.h"
#include "../../Animator/Animator.h"
#include "../../Animator/StateMachineController.h"
#include "./MMDModelResourceData.h"
#include "../../Mesh/ModelMesh.h"
#include "../../GameConponent/GameObject/GameObject.h"
#include "../../Mesh/MeshHeap.h"
#include "../../Model/ModelObject.h"

namespace K3D12 {

	class GraphicsCommandList;
	class PipelineStateObject;
	class ModelConverter;

	struct MMDRendererMaterial
	{
		//�g�U���ˌ� w = alpha
		Vector4 diffuse;
		//���ʔ��ˌ�	w = pow
		Vector4 specular;
		//����
		Vector3 ambient;
		//���Ȕ����F
		Vector3 emissive;
		//�X�t�B�A�}�b�v�̃u�����f�B���O�^�C�v
		int	sphereBlendType;
	};


	//�Q�[���{�҂Ŏg�����f��
	class MMDModel :
		public ModelObject
	{
		friend class ModelConverter;
		friend class ModelPool;

		enum SHADER_ROOT_PARAMATER_INDEX {
			SHADER_ROOT_PARAMATER_INDEX_CAMERA = 0,
			SHADER_ROOT_PARAMATER_INDEX_TRANSFORM,
			SHADER_ROOT_PARAMATER_INDEX_MATERIAL,
			SHADER_ROOT_PARAMATER_INDEX_TEXTURE,
			SHADER_ROOT_PARAMATER_INDEX_TOON_MAP,
			SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE,
		};
	private:
		//���f���̃I���W�i���f�[�^
		std::weak_ptr<MMDModelResourceData>	_resourceData;
	protected:

		Animator							_animator;

	public:

	private:

		Matrix CulcBDEF1(K3D12::WeightDeform& deform);
		Matrix CulcBDEF2(WeightDeform& deform);
		Matrix CulcBDEF4(WeightDeform& deform);
		std::pair<Vector3, float> CulcSDEF(WeightDeform& deform);
		Matrix CulcQDEF(WeightDeform& deform);
		std::pair<float, float> CulcSDEFWaight(Vector3 r0, Vector3 r1);

	protected:

		void	TransformUpdate();

		void	AnimationUpdate();

		void	SkeltonReset();

		void	SetAnimationData(std::string motionPath);

		virtual void RegisterToBundle();

	public:
		//���f��������
		void Initialize();
		//�g�����X�t�H�[���̃A�b�v�f�[�g
		virtual void Update();
		//�A�b�v�f�[�g�֐��ƕ����������A�b�v�f�[�g���s��
		virtual void LateUpdate();
		//���X�g��Draw�R�}���h��ςݍ���(�W�I���g�������_�����O�j
		virtual void Draw();

		virtual Animator& Animator();

		MMDModel();
		virtual ~MMDModel();
	};
}