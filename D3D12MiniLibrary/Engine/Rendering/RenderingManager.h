#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>
#include "../System/D3D12System.h"
#include "../Resource/RenderTarget.h"
#include "../CommandContext/CommandQueue.h"
#include "../CommandContext/GraphicsContextLibrary.h"
#include "RenderingPathObject.h"

namespace K3D12 {

	class RenderingManager
	{
	private:
		//<Key =  pair<MasterPathIndex , MasterPathName> , value = <key = childPathName, value = PathOBJ>
		std::map < std::pair<unsigned int, std::string>, std::map<std::string, RenderingPathObject >> _renderingPathMap;
		std::map < std::string, bool> _masterPathEnableMap;
		RenderTarget _rendertarget;
	public:
	private:
	public:

		HRESULT IntializeRenderingManager(CommandQueue& commandQueue, Factory& factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		//�����_�����O�̎��s
		void ExcutionRendering();

		//�e�t���[���̈�ԍŏ��ɌĂ΂�鏉�����֐�
		void InitializeOnStart();

		//�R�}���h�̎��s�O�ɌĂ΂�鏉�����֐�
		void InitializeOnBeforExcute();

		//�R�}���h�̎��s��ɌĂ΂�鏉�����֐��iOnStart�Ƃ͓����ł��邪�킴�ƕ������Ă���j
		void InitializeOnAfterExcute();

		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(std::string pathName, std::string childPathName = "");
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(unsigned int pathIndex, unsigned int childPathIndex = 0);
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(unsigned int pathIndex, std::string childPathName = "");
		std::weak_ptr<GraphicsCommandList> GetRenderingCommandList(std::string pathName, unsigned int childPathIndex = 0);

		unsigned int GetRenderingPathIndex(std::string pathName, std::string childPathName = "");
		unsigned int GetRenderingPathIndex(std::string pathName, unsigned int childPathIndex = 0);

		std::string  GetRenderingPathName(unsigned int pathIndex, std::string childPathName = "");
		std::string  GetRenderingPathName(unsigned int pathIndex, unsigned int childPathIndex = 0);

		void SetActiveMasterPath(std::string masterPathName, bool isEnable);
		void SetActiveChildPath(std::string masterPathName, std::string childPathName, bool isEnable);


		//�����_�����O�p�X��o�^����֐��A�ԋp�n��StructBinding�Ń����_�����O�p�X���g���R�}���h���X�g�̎�Q�ƂƃC���f�b�N�X�ԍ���Ԃ�
		std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>  RegistRenderingPath(std::string name);
		//�V�K�����_�����O�p�X��pathIndex�ԂɃC���T�[�g����
		std::pair<unsigned int, std::weak_ptr<GraphicsCommandList>>  InsertRenderingPath(std::string name, unsigned int insertPathIndex);

		RenderingManager();
		~RenderingManager();
	};
}