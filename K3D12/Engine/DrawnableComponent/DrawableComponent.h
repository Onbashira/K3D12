#pragma once
#include <memory>
#include "../Util/D3D12Common.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {
	class PipelineStateObject;
	class RootSignature;

	//描画可能コンポネント
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
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle() = 0;
	public:

		DrawableComponent();

		virtual ~DrawableComponent();

		void SetPipelineState(std::weak_ptr<K3D12::PipelineStateObject> pipelineState);

		void SetRootSignature(std::weak_ptr<K3D12::RootSignature> rootSignature);

		void SetMasterCommandList(std::shared_ptr<K3D12::GraphicsCommandList> masterCommandList);

		//PSOとRootSignatureを設定されたマスターコマンドリストにセットする
		void BindingShaderObject();

		void BindingShaderObjectToBundle();

		//描画時呼び出し関数の作成を強制
		virtual void Draw() = 0;
	};
}