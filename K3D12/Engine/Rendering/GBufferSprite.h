#pragma once
#include <map>
#include <memory>
#include <string>
#include "../Resource/VertexBuffer.h"
#include "../Resource/ConstantBuffer.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../PipelineState/PipelineStateObject.h"
#include "../Resource/ShaderResource.h"
#include "../Resource/DepthStencilBuffer.h"
#include "../Window/Window.h"
#include "../Util/Math.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Resource/UnorderedAccessValue.h"

namespace K3D12 {

	class GraphicsCommandList;
	class Window;
	class StructuredBuffer;

	enum class GEOMETRY_TYPE {
		NORMAL,
		SPECULAR,
		ALBEDO,
		DEPTH,
		SPECULER_MAP,
		DIFFUSE_MAP,
		RESULT,
		RESULT2,
		GEOMETRY_TYPE_MAX
	};

	enum class SHADER_PARAMATER_INDEX_LIGHTING {
		CAMERA_INFO,
		NORMAL_MAP,
		SPECULAR_POWER_MAP,
		ABLEDO_MAP,
		DEPTH_MAP,
		LIGHT_BUFFER,
		RESULT,
		SPECULAR_MAP,
		DIFFUSE_MAP,

	};

	enum class SHADER_PARAMATER_INDEX_RESULT {
		ALBEDO_MAP,
		SPECULAR_MAP,
		DIFFUSE_MAP
	};

	class GBufferSprite
	{


	private:
		unsigned int									_threadDimensionX;
		unsigned int									_threadDimensionY;
		Window*											_window;
		unsigned int									_windowWidth;
		unsigned int									_windowHeight;
		Vertex3D										_vertex[4];
		VertexBuffer									_vertexBuffer;
		DescriptorHeap									_geometryShaderHeap;		//NORMAL + ALBEDO + SPEC + Acumration
		DescriptorHeap									_rtvHeap;					//��L�Ɠ���
		DescriptorHeap									_uavHeap;					//SpecularMap , DiffuseMap
		DepthStencil*									_bindedDepthStencil;		//���̃N���X�Ƀo�C���h����Ă���f�v�X�X�e���V��
		CameraInfo										_cameraInfo;				//�W�I���g���p�X�Ŏg�����J�����̏��������Ă���
		ConstantBuffer									_cameraConstantBuffer;		//���C�e�B���O���ɃR���s���[�g�V�F�[�_�֑���f�[�^
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>		_geometryRenderTargets;
		std::map<GEOMETRY_TYPE, ShaderResource>			_geometryResource;
		std::shared_ptr<GraphicsCommandList>			_list;

	public:

	private:
		HRESULT	InitializeVertex();
		HRESULT InitializeResource(unsigned int spriteWidth, unsigned int spriteHeight);
		HRESULT InitializeDescriptorHeap();
		HRESULT InitializeGeometryView(DepthStencil* depth);
		HRESULT CreateLightingPassPso();
	public:

		HRESULT CreateGBuffer(unsigned int spriteWidth, unsigned int spriteHeight, Window* window, std::shared_ptr<GraphicsCommandList> list, DepthStencil* depth);

		D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUHandle(GEOMETRY_TYPE type);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(GEOMETRY_TYPE type);
		D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle(GEOMETRY_TYPE type);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle(GEOMETRY_TYPE type);
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle(GEOMETRY_TYPE type);
		D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle(GEOMETRY_TYPE type);

		Resource* GetGBufferResource(GEOMETRY_TYPE type);

		//�W�I���g���p�X�Ń����_�[�^�[�Q�b�g�Ƃ��Ďg���e�N�X�`���Q���Z�b�g
		void SetGeometryRenderTarget(DepthStencil* depth = nullptr);

		//�J�����̏����Z�b�g����
		void SetCameraInfo(CameraInfo& cameraInfo);

		//�W�I���g���p�X���I�����e�N�X�`�����Z�b�g����
		void SetGeometryResultTexture(DepthStencil* depth = nullptr);

		//���C�e�B���O�Ō��ʂ���������UAV���V�F�[�_�[�ɃZ�b�g����B
		void SetLightingRenderUAV();

		//���C�e�B���O�Ŏg���ł��낤���C�g���V�F�[�_�[�ɃZ�b�g����
		void SetLightStructuredBuffer(UnorderedAccessValue* lightUav);

		//�����_�����O���ʂ�CPU�n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE GetResultTextureCPUHandle();

		//���C�e�B���O�p�X���I�����e�N�X�`���Q���V�F�[�_�[�ɃZ�b�g����
		void SetLightingResult();

		//���C�e�B���O�p�X��񓯊��R���s���[�g�Ŏ��s
		void StartLightingPass(UnorderedAccessValue* lightsBuffer, DepthStencil* depthResource = nullptr);

		//���胊�\�[�X�̃X�e�[�g��J�ڂ�����
		void TransitionResource(GEOMETRY_TYPE type, D3D12_RESOURCE_STATES states);

		//�e�p�X�ɂ����郊�\�[�X�̏�Ԃ�ψʂ�����
		void TransitionGeometryPassStart();
		void TransitionGeometryPassEnd();
		void TransitionLightingPassStart(DepthStencil* depthResource = nullptr);
		void TransitionLightingPassEnd(DepthStencil* depthResource = nullptr);

		void TransitionResultTextureToSR();
		void TransitionResultTextureToUAV();
		void TransitionResultTextureToCopySource();
		void TransitionResultTextureToGeneric();
		void TransitionResultTextureToRT();


		void TransitionResultTexture2ToSR();
		void TransitionResultTexture2ToUAV();
		void TransitionResultTexture2ToCopySource();
		void TransitionResultTexture2ToRT();

		void TransitionResultTexture2ToGeneric();

		//���삷��R�}���h���X�g��ς���
		void BindingCommandList(std::shared_ptr<GraphicsCommandList> newCommandList);
		//�`��Ɏg���f�v�X�X�e���V�����\�[�X��ς���
		void BindingDepthStencill(DepthStencil* depthStencil);

		void Discard();
		void SetHeaps();
		VertexBuffer& GetVertexBuffer();

		GBufferSprite();
		~GBufferSprite();
	};

}