#pragma once
#include <map>
#include <tuple>
#include <type_traits>
#include <memory>

#include "../Util/ManagerComponentBase.h"
#include "RootRenderingPath.h"

namespace K3D12 {

	//�V�[���P�ʂ̃����_�����O�p�X���Ǘ�
	//���ƂŊO���c�[������o�C�i���`���ŃC���|�[�g�ł���悤�ɂ���
	class SceneRenderer
	{
	private:
		std::map<std::string, std::shared_ptr<RootRenderingPath>> _renderingPath;
	public:
	private:
	public:
		//�V�[���P�ʂł̃����_�����O
		void Rendering();
		//�V�[���ɑ΂��ă����_�����O�p�X��ǉ�����
		std::shared_ptr<RootRenderingPath> AddRootRenderingPath(std::string name , RootRenderingPath rootRenderPath);
		//���O���烌���_�����O�p�X���擾����
		std::weak_ptr<K3D12::RootRenderingPath> GetRootRenderingPath(std::string name);
		//���O���烌���_�����O�p�X���폜����
		void EraseRenderingPath(std::string name);
		//���O���烌���_�����O�p�X�̗L��
		void EnableRenderingPath(std::string name);
		//���O���烌���_�����O�p�X�̖���
		void DisableRenderingPath(std::string name);

		SceneRenderer();
		~SceneRenderer();
	};

};