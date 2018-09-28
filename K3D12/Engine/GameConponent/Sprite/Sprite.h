#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../../Util/Math.h"
#include "../../Model/ModelMesh.h"
#include "../../Util/Math.h"

namespace K3D12 {

	class FontData;
	class PipelineStateObject;
	class RootSignature;

	//2DSprite�@���ˉe���e�Ŏg��
	class Sprite :
		public ModelMesh
	{
	private:
		struct SpriteInfo {
			float	alpha; //0.0~1.0
			Vector3 color;
			Vector2 centerOffset; //���S����̃I�t�Z�b�g�x�N�g��
			SpriteInfo() : alpha(1.0f),color(Vector3::one), centerOffset() {};
			~SpriteInfo() {};
		}_info;

		unsigned int  _height;
		unsigned int  _width;

		std::vector<Vertex2D> _vertexes;
		std::vector<unsigned int> _list;
		std::weak_ptr<ShaderResource> _shaderResource;

	protected:

	public:

	private:

	protected:

	public:

		void Draw();
		void DrawString(std::weak_ptr<FontData> font, std::string str);
		void Initializer();
		void RegistBundle();
		void SetPipelineState(PipelineStateObject* pipelineState);
		void SetRootSignature(RootSignature* rootSignature);
		void AttachTexture(std::string path); //�e�N�X�`���A�^�b�`�i���łɃZ�b�g���ꂽ�e�N�X�`���̏��L����j�����ĐV�����e�N�X�`���̏��L���𓾂�j
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
