#pragma once
#include <map>
#include <string>
#include "../Util/D3D12Common.h"



namespace K3D12 {

	class CommandQueue;
	class Fence;
	class PipelineStateObject;
	class DepthStencil;
	class CommandListLibrary;

	class GraphicsCommandList
	{
		friend class CommandListLibrary;
	private:
		//�O���t�B�N�X�R�}���h���X�g
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>	_commandList;
		//�R�}���h���X�g
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		_commandAllocator;
		//�`��Ŏg���f�v�X�X�e���V��
		DepthStencil*										_bindedDepthStencil;
		//�`�掞�ɏ������ރ^�[�Q�b�g�ւ�CPU�������n���h���ƃL�[
		std::map<unsigned int, D3D12_CPU_DESCRIPTOR_HANDLE>	_bindedRenderTargets;
		//�R�}���h���X�g�̖��O
		std::string											_commandListName;
		//�R�}���h�A���P�[�^�̖��O
		std::string											_commandAllocatorName;
		//
		D3D12_COMMAND_LIST_TYPE								_listType;
	public:

	private:

	public:

		GraphicsCommandList();

		~GraphicsCommandList();

		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		HRESULT SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		HRESULT UAVBarrie(ID3D12Resource * resource);

		HRESULT ExcutionCommandList(CommandQueue* queue, Fence* fence = nullptr);

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	GetCommandList()const;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		GetAllocator()const;

		HRESULT						ResetCommandList(ID3D12PipelineState* pInitialState = nullptr);

		HRESULT						ResetAllocator();

		HRESULT						CloseCommandList();

		HRESULT						ConnectingPSO(PipelineStateObject* graphicsPSO);

		HRESULT						BindingDepthStencil(DepthStencil* depth);

		void						AddRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE handles[]);

		void						AddRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE handle, unsigned int index = 0);

		void						UnBindingRenderTargets();

		void						UnBindingRenderTarget(unsigned int renderTargetIndex = 0);

		void						OMSetBindingRenderTargets();

		void						OMSetRenderTargets(unsigned int numRenderTargets, D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, bool RTsSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle = nullptr);

		void						Discard();

		void						SetName(std::string objectName);

		void						SetCommandListName(std::string name);

		void						SetCommandAllocatorName(std::string name);

	};

};