#pragma once
#include <vector>
#include <memory>
#include "../Resource/VertexBuffer.h"
#include "../Resource/IndexBuffer.h"
#include "../Util/D3D12Common.h"
#include "../Util/Math.h"

namespace K3D12 {
	class VertexBuffer;
	class IndexBuffer;
	class GraphcisCommandList;

	//Meshバッファを管理(CPUデータと分離する）
	class MeshBuffer
	{
	private:
		//SysMemに確保される頂点インデックスリスト
		std::unique_ptr<IndexBuffer>	_defaultIBO;
		//デフォルトで使用するVBO Stream0 PerVertex　デフォルトで頂点位置、法線、二次元UVテクスチャ座標をVBOにマップ
		std::unique_ptr<VertexBuffer>	_defaultVBO;
		//ユーザーカスタム型のVBO Stream1 PerVertex
		std::unique_ptr<VertexBuffer>	_customVBO;
	
	public:

	private:

	protected:

	public:		
		
		virtual void InitializeDefaultVBO(ULONG64 size, unsigned int stride, void* defaultVertexDataSrc);

		virtual void InitializeCustomVBO(ULONG64 size, unsigned int stride,void* customVertexDataSrc);

		virtual void InitializeIBO(std::vector<unsigned int>& defaultIndexListData);

		std::vector<D3D12_VERTEX_BUFFER_VIEW> GetMeshVBViews();

		VertexBuffer* GetDefaultVBO();

		VertexBuffer* GetCustomVBO();

		IndexBuffer * GetIBO();

		virtual void DiscardMesh();

		bool UseCustomVertex();

		MeshBuffer();
		virtual ~MeshBuffer();

	};

}