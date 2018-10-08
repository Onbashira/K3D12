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
		IndexBuffer		_indexBuffer;
		//�f�t�H���g�Ŏg�p����VBO Stream0 PerVertex�@�f�t�H���g�Œ��_�ʒu�A�@���A�񎟌�UV�e�N�X�`�����W��VBO�Ƀ}�b�v?
		VertexBuffer	_vertexBuffer;
		//���[�U�[�J�X�^���^��VBO Stream1 PerVertex
		std::vector<VertexBuffer>	_customVBOs;
	
	public:

	private:

	protected:

	public:		
		
		virtual void InitializeVBO(ULONG64 size, unsigned int stride, void* vertexDataSrc);

		virtual void AddCustomVBO(ULONG64 size, unsigned int stride,void* customVertexDataSrc);

		virtual void InitializeIBO(std::vector<unsigned int>& indexListDataSrc);

		std::vector<D3D12_VERTEX_BUFFER_VIEW> GetMeshVBViews();

		VertexBuffer& GetVBO();

		VertexBuffer* GetCustomVBO(unsigned int index);

		IndexBuffer&  GetIBO();

		virtual void Discard();

		bool UseCustomVertex();

		MeshBuffer();
		virtual ~MeshBuffer();

	};

}