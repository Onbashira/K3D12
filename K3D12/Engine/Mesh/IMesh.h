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
	//Mesh�C���^�[�t�F�[�X

	class IMesh
	{
	private:
		//SysMem�Ɋm�ۂ���钸�_�C���f�b�N�X���X�g
		std::unique_ptr<IndexBuffer>	_defaultIBO;
		//�f�t�H���g�Ŏg�p����VBO�iVertex3D�jStream0 PerVertex�@�f�t�H���g�Œ��_�ʒu�A�@���A�񎟌�UV�e�N�X�`�����W��VBO�Ƀ}�b�v
		std::unique_ptr<VertexBuffer>	_defaultVBO;
		//���[�U�[�J�X�^���^��VBO Stream1 PerVertex
		std::unique_ptr<VertexBuffer>	_customVBO;
		//�f�t�H���g���_���{��
		std::vector<Vertex3D>			_defaultVertexes;
		//�f�t�H���g�C���f�b�N�X���X�g�{��
		std::vector<unsigned int>		_defaultIndexList;
		//���[�U�[���_�i�J�X�^�����_�j�͔h���N���X�Ŏ�����������悢�H
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