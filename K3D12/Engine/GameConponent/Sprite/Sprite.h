#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../../Util/Math.h"
#include "../../Model/ModelObject.h"
#include "../../Util/Math.h"

namespace K3D12 {

	class FontData;
	class PipelineStateObject;
	class RootSignature;

	//2DSprite　正射影投影で使う
	class Sprite :
		public ModelObject
	{
	private:
		struct SpriteInfo {
			float	alpha; //0.0~1.0
			Vector3 color;
			Vector2 centerOffset; //中心からのオフセットベクトル
			SpriteInfo()  : alpha(1.0f),color(Vector3::one), centerOffset() {};
			~SpriteInfo() {};
		}_info;

		unsigned int  _height;
		unsigned int  _width;

	protected:

	public:

	private:

	protected:

	public:

		virtual void Draw()override;
		void DrawString(std::weak_ptr<FontData> font, std::string str);
		void Initializer();
		void RegisterToBundle()override;
		void AttachTexture(std::string path); //テクスチャアタッチ（すでにセットされたテクスチャの所有権を破棄して新しいテクスチャの所有権を得る）
		void SetRect(float top, float left, float bottom, float right);
		void SetRect(Vector2 topLeft, Vector2 bottomLeft);
		void SetColor(Vector3 color);
		void SetColor(float r, float g, float b);
		void SetTransparency(float alpha);
		void Translate(Vector2 pos);
		void Move(Vector2 velocity);
		Vector2 GetPos();
		void SetScale(Vector2 scale);
		Vector2 GetScale();
		void Rotate(float deg);
		void Rotate(Quaternion quaternion);
		void Update();
		void LateUpdate();

		Sprite();
		virtual ~Sprite();
	};
}
