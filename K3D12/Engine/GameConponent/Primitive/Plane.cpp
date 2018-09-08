#include "Plane.h"
#include "../../Util/Math.h"
#include "../../Model/ModelMesh.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../RendererMaterial.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"
#include "PrimitiveComponent.h"
#include "PrimitiveObject.h"


constexpr float debugScale = 50.0f;

K3D12::Plane::Plane()
{
}


K3D12::Plane::~Plane()
{
}


void K3D12::Plane::Initializer()
{
	this->_transformStartPoint = 0;
	this->_materialStartPoint = 1;
	this->_textureStartPoint = _materialStartPoint + 1;


	unsigned int sufaceCount = 6;
	unsigned int planeVertex = 4;
	Vertex3D plane[4];

	for (unsigned int i = 0; i < planeVertex; ++i) {
		plane[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f), 0.0f, -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f));
		plane[i].normal = Vector3::up;
		plane[i].texCoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
		this->_info._vertexes.push_back(plane[i]);


	}
	unsigned int planeList[] = { 0,1,2,1,3,2 };
	for (unsigned int listIndex = 0; listIndex < sufaceCount; ++listIndex) {
		this->_info._list.push_back(planeList[listIndex]);
	}

	// Resource Creation
	{
		this->_indexBuffer->Create(this->_info._list.size() * sizeof(unsigned int), sizeof(unsigned int), &this->_info._list[0]);
		this->_vertexBuffer->Create(this->_info._vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &this->_info._vertexes[0]);

		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		_materialBuffer.Create(Util::Alignment256Bytes(sizeof(RendererMaterial)));
		_materialBuffer.SetName("PrimitveMaterialBuffer");
	}

	//Material Update
	this->_info._materialInfo.ambient = Vector3::zero;
	this->_info._materialInfo.diffuse = Vector4(Vector3::one, 1.0f);
	this->_info._materialInfo.specular = Vector4(Vector3::zero, 0.000f);
	this->_info._materialInfo.emissive = Vector3(Vector3::one) / 10.0f;
	_materialBuffer.Update(&this->_info._materialInfo, sizeof(RendererMaterial), 0);

	//TransformUpdate
	SetScale(Vector3::one);
	Translate(Vector3::zero);
	SetRotation(Quaternion::CreateFromEulerAngles(Vector3::zero));
	GameObject::UpdateTransformBuffer();

	//HeapCreation

	// 0 = Transform,1 = materialInfo,2 = ShaderTexture(default is null)
	_heap->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);

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
		RegistBundle();
	}
}

void K3D12::Plane::RegistBundle()
{

	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindingIndexBuffer(&_bundleList);
	BindingVertexBuffer(&_bundleList);
	BindingDescriptorHeaps(&_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(1, _heap->GetGPUHandle(_transformStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(2, _heap->GetGPUHandle(_materialStartPoint));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(3, _heap->GetGPUHandle(_textureStartPoint));

	_bundleList.GetCommandList()->DrawIndexedInstanced(static_cast<unsigned int>(this->_info._list.size()), 1, 0, 0, 0);

	_bundleList.GetCommandList()->Close();
}

void K3D12::Plane::SetPipelineState(PipelineStateObject * pipelineState)
{
	_pipelineState.reset(pipelineState);
}

void K3D12::Plane::SetRootSignature(RootSignature * rootSignature)
{
	_rootSignature.reset(rootSignature);
}

void K3D12::Plane::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(0, D3D12System::GetInstance().GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	ID3D12DescriptorHeap* heap[] = { _heap->GetPtr() };
	this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

void K3D12::Plane::Update()
{
	GameObject::UpdateTransformBuffer();

}

void K3D12::Plane::LateUpdate()
{
}

void K3D12::Plane::AttachTexture(std::string pathName)
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

	RegistBundle();
}

void K3D12::Plane::SetUV(float compressionRatio)
{
	for (unsigned int surface = 0; surface < _info._vertexes.size(); surface += 4) {
		_info._vertexes[surface + 1].texCoord = Vector2(compressionRatio, 0.0f);
		_info._vertexes[surface + 2].texCoord = Vector2(0.0f, compressionRatio);
		_info._vertexes[surface + 3].texCoord = Vector2(compressionRatio, compressionRatio);
	}
	this->_vertexBuffer->Map(0, nullptr);
	this->_vertexBuffer->Update(&this->_info._vertexes[0], this->_info._vertexes.size() * sizeof(Vertex3D), 0);
	this->_vertexBuffer->Unmap(0, nullptr);
}
