#include "PMDModel.h"


K3D12::PMDModel::PMDModel()
{

}

K3D12::PMDModel::~PMDModel()
{

}

//HRESULT PMDModel::Draw(GraphicsCommandList * commandList)
//{
//	//�{�[���ƃ��[�V�����f�[�^�A�ό`���@�AIK�A�ɂ�钸�_�ό`�X�e�[�W�ƁA�v�Z�X�e�[�W�����B
//
//	//���f���A�b�v�f�[�g
//	{
//		//�萔�o�b�t�@���V�F�[�_�̃��W�X�^�ɃZ�b�g
//		//�V�[���萔�@���W
//		ID3D12DescriptorHeap* sceneHeaps[] = { _transformBufferHeap.GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(sceneHeaps), sceneHeaps);
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(0, _transformBufferHeap.GetGPUHandle(0));
//		//�C���f�b�N�X���g�p���A�g���C�A���O�����X�g��`��
//		commandList->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		commandList->GetCommandList()->IASetVertexBuffers(0, 1, &_vertexBuffer.GetView());
//		commandList->GetCommandList()->IASetIndexBuffer(&_indexBuffer.GetView());
//	}
//	UINT offset = 0;
//
//	for (size_t i = 0; i < _materials.size(); ++i)
//	{
//		// �e�N�X�`���ƃ}�e���A����ݒ�.
//
//		//�e�N�X�`���̐ݒ�
//
//
//		ID3D12DescriptorHeap* texHeaps[] = { _textureHeap.lock()->GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(texHeaps), texHeaps);
//		////���[�g�p�����[�^�ɍ��킹�Ă�
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(3, _textureHeap.lock()->GetGPUHandle(static_cast<UINT>(i)));// �R�}���h���X�g�ɐݒ�
//
//		//�}�e���A���̐ݒ�
//		ID3D12DescriptorHeap* mat_heaps[] = { _materialBufferHeaps.GetPtr() };
//		commandList->GetCommandList()->SetDescriptorHeaps(_countof(mat_heaps), mat_heaps);
//		commandList->GetCommandList()->SetGraphicsRootDescriptorTable(2, _materialBufferHeaps.GetGPUHandle(static_cast<UINT>(i)));					//�}�e���A��
//
//		// �`��R�}���h�𐶐�.
//
//		commandList->GetCommandList()->DrawIndexedInstanced(_materials[i].faceVertex, 1, offset, 0, 0);
//
//		offset += _materials[i].faceVertex;
//	}
//	return S_OK;
//}
//
//HRESULT PMDModel::Update(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
//{
//	//���f���A�b�v�f�[�g
//	//{
//	//	static constexpr float PI = 3.14159265358979323846264338f;
//	//	static int cnt{};
//	//	++cnt;
//
//
//	//	//�J�����̐ݒ�(���Ƃň����w��ł���悤�ɂ���)
//	//	_transformData.View = view;
//	//	_transformData.Proj = projection;
//
//	//	//�I�u�W�F�N�g�̉�]�̐ݒ�
//	//	_transformData.World = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(static_cast<float>(cnt % 1800)) / 5.0f);
//
//	//	_transformBuffer.Update(&_transformData, sizeof(_transformData), 0);
//
//	//	//�{�[���A�b�v�f�[�g
//	//	/*std::fill(_boneTree.nodeMatrix.begin(), _boneTree.nodeMatrix.end(), DirectX::XMMatrixIdentity());
//	//	BendFromMotion();
//
//	//	_boneMatrixBuffer.Update(&_boneTree.nodeMatrix[0], _boneTree.nodeMatrix.size() *sizeof(DirectX::XMMATRIX),0);*/
//	//}
//	return S_OK;
//}
//
//HRESULT PMDModel::AnimationUpdate()
//{
//	return E_NOTIMPL;
//}
//
//void PMDModel::Discard()
//{
//	_vertexBuffer.Discard();
//	_indexBuffer.Discard();
//	_materialBuffer.Discard();
//
//	_vertices.clear();
//	_verticesIndex.clear();
//	_materials.clear();
//
//}
//
//std::vector<PMDMaterial>& PMDModel::GetMaterials()
//{
//	return _materials;
//}

