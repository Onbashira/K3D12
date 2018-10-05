#include "Cube.h"
#include "../../Util/Math.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../RendererMaterial.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"
#include "../../Resource/UnorderedAccessValue.h"
#include "PrimitiveComponent.h"

constexpr float debugScale = 10.0f;

K3D12::Cube::Cube()
{

}

K3D12::Cube::Cube(Vector3 pos, Vector3 scale)
{
	Translate(pos);
	SetScale(scale);
}


K3D12::Cube::~Cube()
{

}

void K3D12::Cube::Initializer()
{
	SetTransformDescStartIndex(0);
	SetMaterialDescStartIndex();
	SetTextureDescStartIndex();


	unsigned int sufaceCount = 6;
	unsigned int planeVertex = 4;
	Vertex3D plane[4];
	for (unsigned int i = 0; i < planeVertex; ++i) {
		for (unsigned int i = 0; i < planeVertex; ++i) {
			plane[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f) / 2.0f, -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f) / 2.0f, -1.0f / 2.0f);
			plane[i].normal = Vector3::back;
			plane[i].texCoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
		}
	}
	unsigned int planeList[] = { 0,1,2,1,3,2 };

	// Cube Create Section
	//Žl–Ê
	for (unsigned int i = 0; i < sufaceCount - 2; ++i) {
		Vertex3D p;
		Quaternion q = Quaternion::CreateFromAxisAngles(Vector3::up, F_PIDIV2 * i);
		for (unsigned int v = 0; v < planeVertex; v++) {
			p.normal = Vector3::Rotate(plane[v].normal, q);
			p.pos = Vector3::Rotate(plane[v].pos, q);

			p.texCoord = plane[v].texCoord;
			GetDefaultVertexData().push_back(p);
		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			GetDefaultIndexListData().push_back(planeList[listIndex] + (4 * i));
		}
	}
	//’ê–Êã–Ê
	for (unsigned int i = sufaceCount - 2; i < sufaceCount; ++i) {
		Vertex3D p;
		float rad = F_PIDIV2 + F_PI * (i - (sufaceCount - 2));
		Quaternion q = Quaternion::CreateFromAxisAngles(Vector3::right, rad);
		for (unsigned int v = 0; v < planeVertex; v++) {
			p.normal = Vector3::Rotate(plane[v].normal, q);
			p.pos = Vector3::Rotate(plane[v].pos, q);
			p.texCoord = plane[v].texCoord;
			GetDefaultVertexData().push_back(p);

		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			GetDefaultIndexListData().push_back(planeList[listIndex] + (4 * i));
		}
	}

	// Resource Creation
	{
		this->GetDefaultIBO()->Create(GetDefaultIndexListData().size() * sizeof(unsigned int), sizeof(unsigned int), &this->GetDefaultIndexListData()[0]);
		this->GetDefaultIBO()->SetName("CubeIndexBuffer");

		this->GetDefaultVBO()->Create(GetDefaultVertexData().size() * sizeof(Vertex3D), sizeof(Vertex3D), &this->GetDefaultVertexData()[0]);
		GetDefaultVBO()->SetName("CubeVertexBuffer;");
		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		GetMaterialBufffer().Create(Util::Alignment256Bytes(sizeof(RendererMaterial)));
		GetMaterialBufffer().SetName("PrimitveMaterialBuffer");
	}

	//Material Update
	this->_info._materialInfo.ambient = Vector3::zero;
	this->_info._materialInfo.diffuse = Vector4(Vector3::one, 1.0f);
	this->_info._materialInfo.specular = Vector4(Vector3::zero, 0.0f);
	this->_info._materialInfo.emissive = Vector3(Vector3::one) / 10.0f;
	GetMaterialBufffer().Update(&this->_info._materialInfo, sizeof(RendererMaterial), 0);


	//TransformUpdate
	SetScale(Vector3::one);
	Translate(Vector3::zero);
	SetRotation(Quaternion::CreateFromEulerAngles(Vector3::zero));
	GameObject::UpdateTransformBuffer();


	//HeapCreation

	// 0 = Transform,1 = materialInfo,2 = ShaderTexture(default is null)
	GetMeshHeap().GetHeap().Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
	GetMeshHeap().GetHeap().SetName("CubeHeap");
	//ViewCreation
	{

		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _transformBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

			_transformBuffer.CreateView(view, _heap->GetCPUHandle(_transformStartPoint));
		}

		{

			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _materialBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(RendererMaterial)));

			_materialBuffer.CreateView(view, _heap->GetCPUHandle(_materialStartPoint));
		}

		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = _info._shaderResource.lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			_info._shaderResource.lock()->CreateView(srvDesc, _heap->GetCPUHandle(_textureStartPoint));
		}
	}
	//SetBundleDrawCall
	{
		RegisterToBundle();
	}
}

void K3D12::Cube::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(0, K3D12::GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	ID3D12DescriptorHeap* heap[] = { _heap->GetPtr() };
	this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

void K3D12::Cube::RegisterToBundle()
{
	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindIndexBufferToBundle();
	BindVertexBufferToBundle();

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(1, _heap->GetGPUHandle(_transformStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(2, _heap->GetGPUHandle(_materialStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(3, _heap->GetGPUHandle(_textureStartPoint));

	_bundleList.GetCommandList()->DrawIndexedInstanced(static_cast<unsigned int>(this->_info._list.size()), 1, 0, 0, 0);

	_bundleList.GetCommandList()->Close();
}

void K3D12::Cube::InitializeCustomVBO(void ** customVertexDataSrc)
{
}

void K3D12::Cube::Update()
{
	GameObject::UpdateTransformBuffer();
}

void K3D12::Cube::AttachTexture(std::string pathName)
{
	_info.AttachTexture(pathName, _heap->GetCPUHandle(_textureStartPoint));
	_heap->Discard();
	_heap->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
	_heap->SetName("CubeHeap");
	//ViewCreation
	{

		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _transformBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

			_transformBuffer.CreateView(view, _heap->GetCPUHandle(_transformStartPoint));
		}

		{

			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _materialBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(RendererMaterial)));

			_materialBuffer.CreateView(view, _heap->GetCPUHandle(_materialStartPoint));
		}

		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = _info._shaderResource.lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			_info._shaderResource.lock()->CreateView(srvDesc, _heap->GetCPUHandle(_textureStartPoint));
		}
	}
	_bundleList.ResetAllocator();
	_bundleList.ResetCommandList();

	RegisterToBundle();
}

void K3D12::Cube::SetUV(float compressionRatio)
{
	for (unsigned int surface = 0; surface < GetDefaultVertexData().size();surface+=4) {
		GetDefaultVertexData()[surface + 1].texCoord = Vector2(compressionRatio,0.0f);
		GetDefaultVertexData()[surface + 2].texCoord = Vector2(0.0f, compressionRatio);
		GetDefaultVertexData()[surface + 3].texCoord = Vector2(compressionRatio, compressionRatio);
	}
	this->GetDefaultVBO()->Map(0,nullptr);
	this->GetDefaultVBO()->Update(&this->GetDefaultVertexData()[0], this->GetDefaultVertexData().size() * sizeof(Vertex3D), 0);
	this->GetDefaultVBO()->Unmap(0,nullptr);
}
