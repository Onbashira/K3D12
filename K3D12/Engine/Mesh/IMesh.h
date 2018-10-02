#pragma once
#include <vector>
#include <memory>
#include "../Resource/VertexBuffer.h"
#include "../Util/D3D12Common.h"
#include "../Util/Math.h"

namespace K3D12 {
	class VertexBuffer;
	class IndexBuffer;
	class GraphcisCommandList;
	//Meshインターフェース

	class IMesh
	{
	private:
		//SysMemに確保される頂点インデックスリスト
		std::unique_ptr<IndexBuffer>	_defaultIBO;
		//デフォルトで使用するVBO（Vertex3D）Stream0 PerVertex　デフォルトで頂点位置、法線、二次元UVテクスチャ座標をVBOにマップ
		std::unique_ptr<VertexBuffer>	_defaultVBO;
		//ユーザーカスタム型のVBO Stream1 PerVertex
		std::unique_ptr<VertexBuffer>	_customVBO;
		//デフォルト頂点情報本体
		std::vector<Vertex3D>			_defaultVertexes;
		//デフォルトインデックスリスト本体
		std::vector<unsigned int>		_defaultIndexList;
		//ユーザー頂点（カスタム頂点）は派生クラスで持たせる方がよい？
	public:
	private:
	public:
		std::vector<VertexBuffer&> GetMeshVBOs();
		void SetDefaultVBO(VertexBuffer* defaultVBO);
		VertexBuffer* GetDefaultVBO();
		void SetupCustomVBO(VertexBuffer* customVBO);
		VertexBuffer* GetCustomVBO();
		void SetDefaultIBO(IndexBuffer* defaultIBO);
		IndexBuffer * GetDefaultIBO();
		std::vector<Vertex3D>& GetDefaultVertexData();
		std::vector<unsigned int>& GetDefaultIndexListData();

		virtual void InitializeDefaultVBO(std::vector<Vertex3D>& defaultVertexData) = 0;
		virtual void InitializeCustomVBO(void** customVertexDataSrc) = 0;
		virtual void InitializeDefaultIBO(std::vector<unsigned int>& defaultVertexData) = 0;

		virtual void BindingVertexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;
		virtual void BindingIndexBuffer(std::weak_ptr<K3D12::GraphicsCommandList> list) = 0;

		virtual void DiscardMesh();

		bool UseCustomVertex();
		IMesh();
		virtual ~IMesh();

	};

}