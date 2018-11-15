#pragma once
#include <memory>
#include <vector>
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/Resource.h"


namespace K3D12 {
	class GraphicsCommandList;
	class CommandQueue;
	class Factory;
	class Window;

	class SwapChain
	{
		unsigned int										_bufferNum;

		unsigned int										_currentIndex;

		Microsoft::WRL::ComPtr<IDXGISwapChain4>				_swapChain;

		std::vector<Resource>								_rtResource;

		DescriptorHeap										_rtHeap;

	public:

	private:

		HRESULT CreateSwapChain(CommandQueue& commandQueue, Factory& factory, Window& window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum);

		HRESULT CreateRenderTargets(unsigned int bufferNum = 2);

	public:

		SwapChain();

		~SwapChain();

		HRESULT Create(CommandQueue& commandQueue, Factory& factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		unsigned int GetBufferNum();

		unsigned int GetCurrentBuckBuffer();

		HRESULT SetStatePresent(std::shared_ptr<GraphicsCommandList> list);

		HRESULT SetStateRenderTarget(std::shared_ptr<GraphicsCommandList> list);

		HRESULT	SetStateCopyDest(std::shared_ptr<GraphicsCommandList> list);

		HRESULT	SetStateGenericRead(std::shared_ptr<GraphicsCommandList> list);

		HRESULT	CopyToRenderTarget(std::shared_ptr<GraphicsCommandList> list, Resource* pSrc);

		void SetRenderTarget(std::shared_ptr<GraphicsCommandList> list, D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle = nullptr);

		void ClearScreen(std::shared_ptr<GraphicsCommandList> list);

		void FlipScreen();

		HRESULT Present(unsigned int sysncInterval = 1, unsigned int flags = 0);

		void Discard();

	};

};