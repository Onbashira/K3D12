#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <wrl.h>

#include "../Util/D3D12Common.h"
#include "../Window/Window.h"
#include "../Device/D3D12Device.h"
#include "../Factory/Factory.h"
#include "../AsyncComponent/Fence.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "../CommandContext/CommandQueue.h"
#include "../SwapChain/SwapChain.h"
#include "../Input/InputManager.h"
#include "../GameConponent/Camera/Camera.h"
#include "../Model/ModelPool.h"
#include "../Input/GamePadManager.h"
#include "../Rendering/GBufferSprite.h"
#include "../GameConponent/Primitive/PrimitiveCreater.h"


namespace K3D12 {

	class D3D12System
	{

	private:
		
		inline static D3D12System*		_instance = nullptr;
		
		D3D12Device						_device;
		
		Factory							_factory;
		
		Fence							_fence;
		
		Window							_window;
		
		UINT							_windowWidth;
		
		UINT							_windowHeight;
		
		std::wstring					_appClassName;
		
		CommandQueue					_commandQueue;

		CommandQueue					_computeQueue;

		CommandQueue					_copyQueue;
		
		SwapChain						_renderTarget;
		
		InputManager					_inputManager;
		
		Camera							_mainCamera;
		
		ModelPool						_modelPool;
		
		PrimitiveCreater				_primitiveCreater;
		
		GBufferSprite					_geometryBufferSprite;

	public:
	
	private:

		D3D12System();

		HRESULT InitializeWindow();

		HRESULT InitializeD3D12(unsigned int bufferCount = 2, bool useWarpDevice = false);

		HRESULT InitializeDevice(bool useWarpDevice = false);

		HRESULT InitializeDXGIFactory();

		HRESULT InitializeRenderTargets(unsigned int bufferCount = 2);

		HRESULT InitializeCommandQueue();

		HRESULT InitializeComputeQueue();

		HRESULT InitializeCopyQueue();

		HRESULT InitializeCommandList();

		HRESULT InitializeFence();

		HRESULT InitializePSO();

		HRESULT InitializeSpritePSO();

		HRESULT InitializePrimitivePSO();

		HRESULT InitializeFxAAPSO();

		HRESULT InitializeModelTransparency();

		HRESULT InitializeModelOpacity();

		HRESULT	InitializeModelPool();

		HRESULT	InitializePrimitiveCreater();

		HRESULT InitializeMainCamera();

		HRESULT CommandListConnecting();

		HRESULT BindingGBufferRenderTarget();

		HRESULT InitiazlieGBufferSprite();

		HRESULT InitializeGPUParticle();


		void TermWindow();

		void TermD3D12();

		friend  void ScreenFlip();

		friend  void ClearScreen();

		friend  void Drawing2DStep();

		friend  void Drawing3DStep();

	public:

		~D3D12System();

		friend  void Create();

		static D3D12System& GetInstance();

		friend  void Destroy();

		friend  HRESULT Initialize(UINT windowWidth, UINT windowHeight, UINT backBufferCount, bool useWarpDevice );

		friend  HRESULT Initialize(UINT backBufferCount , bool useWarpDevice );

		static void InitializeCamera(CameraMode mode , const Vector3& pos, const Vector3 & target, const Vector3& up, float nearClip, float farClip);

		friend  void SetWindowSize(UINT widths, UINT height);

		friend  void SetWindowName(std::wstring name);

		friend  int MessageLoop();

		friend  HRESULT LoadTexture(std::string path);

		friend  std::weak_ptr<MMDModel>LoadModel(std::string modelPath);

		static std::shared_ptr<GamePad> GetController(int padID = 0);

		static void	 InitializeController(int controllerCount = 4);

		static  Window& GetWindow();

		static void InputUpdate();

		static HRESULT CreatePSO(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3DBlob* rootSignature = nullptr);

		static HRESULT CreatePSO(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ID3DBlob* rootSignature = nullptr);

		static HRESULT CreateCommandList(std::string name, unsigned int nodeMask, const D3D12_COMMAND_LIST_TYPE& type);

		static std::shared_ptr<PipelineStateObject> GetPSO(std::string name);

		static std::shared_ptr<GraphicsCommandList>	GetCommandList(std::string name);

		static std::shared_ptr<GraphicsCommandList>	GetMasterCommandList();

		static std::shared_ptr<GraphicsCommandList>	GetComputeCommandList();

		static std::shared_ptr<GraphicsCommandList>	GetCopyCommandList();

		static  CommandQueue& GetMasterCommandQueue();

		static  CommandQueue& GetMasterComputeQueue();

		static  CommandQueue& GetMasterCopyQueue();

		friend  Camera& GetCamera();

		friend  ID3D12Device * GetDevice();

		friend  IDXGIFactory4* GetFactory();

		friend  Fence*		   GetFence();

		friend  InputManager&  Input();

		friend void SetMainRenderTarget(std::weak_ptr<GraphicsCommandList> list,D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle);

	};

	//global function

	 void Create();

	 void Destroy();

	 HRESULT Initialize(UINT windowWidth, UINT windowHeight, UINT backBufferCount = 2, bool useWarpDevice = false);

	 HRESULT Initialize(UINT backBufferCount = 2, bool useWarpDevice = false);

	 void ScreenFlip();

	 void ClearScreen();

	 Camera& GetCamera();

	 ID3D12Device * GetDevice();

	 IDXGIFactory4* GetFactory();

	 Fence*		   GetFence();

	 InputManager&  Input();

	 void SetWindowSize(UINT widths, UINT height);

	 void SetWindowName(std::wstring name);

	 void SetMainRenderTarget(std::weak_ptr<GraphicsCommandList> list,D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle);

	 int MessageLoop();

	 std::weak_ptr<MMDModel> LoadModel(std::string modelPath);


#ifndef GET_DEVICE
#define GET_DEVICE K3D12::GetDevice()
#endif // !GET_DEVICE

#ifndef CHECK_RESULT 
#define CHECK_RESULT(result) if(FAILED(result)){return result;}
#endif // !CHECK_RESULT

#ifndef GET_SYSTEM_FENCE 
#define GET_SYSTEM_FENCE K3D12::GetFence()
#endif // !GET_DEVICE
}