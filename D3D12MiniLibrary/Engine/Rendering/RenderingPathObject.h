#pragma once
#include <memory>
#include <functional>
#include "Math.h"
#include "../Util/D3D12Common.h"


namespace K3D12 {
	class RenderingManager;
	class PipelineStateObject;
	class GraphicsCommandList;
	class CommandQueue;
	class Fence;

	//�����_�����O���ɕ`�施�߂�ςރR�}���h�R���e�L�X�g���u�����邽�߂̃N���X
	class RenderingPathObject
	{
		friend class  RenderingManager;
	private:

		unsigned int  _pathIndex;
		std::string   _pathName;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onStartInitializerFunc;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onBeforExcuteInitializerFunc;
		std::function<void(std::weak_ptr<GraphicsCommandList>)> _onAfterExcuteInitializerFunc;
		std::shared_ptr<GraphicsCommandList> _masterCommandList;
		bool _isEnable;

	public:

	private:

	public:

		//�e�t���[���̈�ԍŏ��ɌĂ΂�鏉�����֐�
		void OnStartInitializer(std::weak_ptr<GraphicsCommandList> list);

		//�R�}���h�̎��s�O�ɌĂ΂�鏉�����֐�
		void OnBeforExcuteInitializer(std::weak_ptr<GraphicsCommandList> list);

		//�R�}���h�̎��s��ɌĂ΂�鏉�����֐��iOnStart�Ƃ͓����ł��邪�킴�ƕ������Ă���j
		void OnAfterExcuteInitializer(std::weak_ptr<GraphicsCommandList> list);

		//�e�C�j�V�����C�U�̃Z�b�g
		void SetOnStartInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//�e�C�j�V�����C�U�̃Z�b�g
		void SetOnBeforExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//�e�C�j�V�����C�U�̃Z�b�g
		void SetOnAfterExcuteInitializer(std::function<void(std::weak_ptr<GraphicsCommandList> list)> initilalizer);

		//���̃p�X�̗L�������̐؂�ւ�
		void SetEnable(bool isEnable);

		RenderingPathObject();
		~RenderingPathObject();
	};
}