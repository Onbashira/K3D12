#pragma once
#include <map>
#include <tuple>
#include <type_traits>
#include <memory>

#include "../Util/ManagerComponentBase.h"
#include "RootRenderingPath.h"

namespace K3D12 {

	//シーン単位のレンダリングパスを管理
	//あとで外部ツールからバイナリ形式でインポートできるようにする
	class SceneRenderer
	{
	private:
		std::map<std::string, std::shared_ptr<RootRenderingPath>> _renderingPath;
	public:
	private:
	public:
		//シーン単位でのレンダリング
		void Rendering();
		//シーンに対してレンダリングパスを追加する
		std::shared_ptr<RootRenderingPath> AddRootRenderingPath(std::string name , RootRenderingPath rootRenderPath);
		//名前からレンダリングパスを取得する
		std::weak_ptr<K3D12::RootRenderingPath> GetRootRenderingPath(std::string name);
		//名前からレンダリングパスを削除する
		void EraseRenderingPath(std::string name);
		//名前からレンダリングパスの有効
		void EnableRenderingPath(std::string name);
		//名前からレンダリングパスの無効
		void DisableRenderingPath(std::string name);

		SceneRenderer();
		~SceneRenderer();
	};

};