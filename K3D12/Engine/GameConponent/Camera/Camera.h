#pragma once
#include "../../Util/D3D12Common.h"
#include "../GameObject/GameObject.h"
#include "../../Resource/ConstantBuffer.h"
#include "../../Resource/DepthStencilBuffer.h"
#include "../../Input/InputManager.h"
#include "../../Util/Math.h"

namespace K3D12 {

	class GraphicsCommandList;

	enum class CameraMode {
		Perspective,
		Orthogonal
	};

	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		float  windowWidth;
		float  windowHeight;
	};

	class Camera :
		public GameObject
	{
	private:
		CameraMode		_mode;
		
		float			_fov;
		float			_aspectRatio;
		float			_near;
		float			_far;

		float			_windowWidth;
		float			_windowHeight;

		Matrix			_projection;

		ConstantBuffer	_cameraMatrixBuffer;
		DepthStencil	_depthStencillRersource;

		CameraInfo		_info;
	public:

	private:
		
		HRESULT CreateBuffer();
		
		HRESULT InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
		
		HRESULT initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
	public:
		CameraMode		GetMode();

		const Matrix&	GetProjection();
	
		const Matrix	GetViewProjection();
	
		CameraInfo		GetCameraInfo();
	
		ConstantBuffer& GetCameraBuffer();
	
		DepthStencil&	GetDepthStencil();

		float GetFov();
		
		float GetAspectratio;
		
		float GetNearClip();
		
		float GetFarClip();

		void Update();

		void DebugMove(InputManager& input);
		
		void DebugRotate(InputManager& input);

		void ChangeCameraMode(CameraMode mode);

		void InitializeCamera(CameraMode type, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
		
		void InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
		
		HRESULT InitializeCameraDepthStencill(DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight);

		void SetCameraParamater(std::weak_ptr<GraphicsCommandList> list, unsigned int paramaterIndex = 0);
		
		void Discard();

		Camera();

		~Camera();
	};

}