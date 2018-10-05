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

	//Mesh�o�b�t�@���Ǘ�(CPU�f�[�^�ƕ�������j
	class MeshBuffer
	{
	private:
		//SysMem�Ɋm�ۂ���钸�_�C���f�b�N�X���X�g
		std::unique_ptr<IndexBuffer>	_defaultIBO;
		//�f�t�H���g�Ŏg�p����VBO Stream0 PerVertex�@�f�t�H���g�Œ��_�ʒu�A�@���A�񎟌�UV�e�N�X�`�����W��VBO�Ƀ}�b�v
		std::unique_ptr<VertexBuffer>	_defaultVBO;
		//���[�U�[�J�X�^���^��VBO Stream1 PerVertex
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