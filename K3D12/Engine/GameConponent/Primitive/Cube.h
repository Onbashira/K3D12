#pragma once
#include "../../Model/ModelObject.h"
#include "PrimitiveComponent.h"
#include "../../Model/ModelObject.h"

namespace K3D12 {
	class Cube :
		public ModelObject
	{
	private:
	protected:
		PrimitiveComponent _info;
	public:
	private:
	protected:
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle()override;
	public:

		virtual void Update()override;
		//描画時呼び出し関数
		virtual void Draw()override;
		//モデルのセットアップ
		void Initializer();
		void AttachTexture(std::string pathName);
		void SetUV(float compressionRatio);

		Cube();
		Cube(Vector3 pos, Vector3 scale);
		~Cube();
	};
}