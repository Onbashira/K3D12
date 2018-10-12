#pragma once
#include <string>

namespace K3D12 {
	class Scene
	{
	private:
		std::string _sceneName;
	protected:

	public:

	private:

	protected:

	public:
		Scene();

		virtual ~Scene();	

		virtual void ChangeScene() = 0;

		virtual void Rendering() = 0;

		virtual void Update() = 0;

		virtual void LateUpdate() = 0;

		virtual void FixUpdate() = 0;

		void SetName(std::string name);

		std::string GetName();

	};
}