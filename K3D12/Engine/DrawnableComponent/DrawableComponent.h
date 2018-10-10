#pragma once
#include <memory>
#include "../Util/D3D12Common.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {
	class PipelineStateObject;
	class RootSignature;

	//�`��\�R���|�l���g
	class DrawableComponent
	{
	private:
	protected:

		std::weak_ptr<K3D12::PipelineStateObject>		_pipelineState;

		std::weak_ptr<K3D12::RootSignature>				_rootSignature;

		std::weak_ptr<K3D12::GraphicsCommandList>		_commandList;

		K3D12::GraphicsCommandList						_bundleList;

	public:
	private:
	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle() = 0;
	public:

		DrawableComponent();

		virtual ~DrawableComponent();

		void SetPipelineState(std::weak_ptr<K3D12::PipelineStateObject> pipelineState);

		void SetRootSignature(std::weak_ptr<K3D12::RootSignature> rootSignature);

		void SetMasterCommandList(std::shared_ptr<K3D12::GraphicsCommandList> masterCommandList);

		//PSO��RootSignature��ݒ肳�ꂽ�}�X�^�[�R�}���h���X�g�ɃZ�b�g����
		void BindingShaderObject();

		void BindingShaderObjectToBundle();

		//�`�掞�Ăяo���֐��̍쐬������
		virtual void Draw() = 0;
	};
}