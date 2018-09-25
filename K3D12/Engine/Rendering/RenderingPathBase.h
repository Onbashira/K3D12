#pragma once
#include <memory>
#include <functional>
#include"../CommandContext/GraphicsCommandList.h"
#include "../CommandContext/CommandQueue.h"
namespace K3D12 {

	//レンダリンパスの最小単位
	class RenderingPathBase
	{
	private:

		bool	_isEnable;
		std::weak_ptr<GraphicsCommandList> _commandList;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onInitFS;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onInitBE;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onInitAE;

	public:

	private:

	public:

		virtual void InitBeforExcution();
		virtual void InitAfterExcution();
		virtual void InitFrameStart();

		virtual void SetBeforExcutionInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)> function);
		virtual void SetAfterExcutionInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)>function);
		virtual void SetFrameStartInitializer(std::function<void(std::weak_ptr<GraphicsCommandList>)>function);

		virtual void SetEnable(bool enable);
		virtual void SetCommandList(std::weak_ptr<GraphicsCommandList> list);

		bool IsEnable();

		RenderingPathBase();
		virtual ~RenderingPathBase();
	};
}
