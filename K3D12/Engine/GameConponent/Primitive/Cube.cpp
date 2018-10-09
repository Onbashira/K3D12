#include "Cube.h"
#include "../../Util/Math.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../RendererMaterial.h"
#include "PrimitiveComponent.h"
#include "PrimitiveObject.h"
#include "../../TextureComponent/TextureManager.h"

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
	this->GetMeshHeap().SetTransformDescStartIndex(0);
	this->GetMeshHeap().SetMaterialDescStartIndex();
	this->GetMeshHeap().SetTextureDescStartIndex();


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
			_vertexes.push_back(p);
		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			_indexList.push_back(planeList[listIndex] + (4 * i));
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
			_vertexes.push_back(p);

		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			_indexList.push_back(planeList[listIndex] + (4 * i));
		}
	}

	// Resource Creation
	{
		this->GetMeshBuffer().GetIBO().Create(_indexList.size() * sizeof(unsigned int), sizeof(unsigned int), &this->_indexList[0]);
		this->GetMeshBuffer().GetIBO().SetName("CubeIndexBuffer");

		this->GetMeshBuffer().GetVBO().Create(_vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &this->_vertexes[0]);
		this->GetMeshBuffer().GetVBO().SetName("CubeVertexBuffer;");

		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		this->GetMeshHeap().GetMaterialBufffer().Create(Util::Alignment256Bytes(sizeof(RendererMaterial)));
		this->GetMeshHeap().GetMaterialBufffer().SetName("PrimitveMaterialBuffer");
	}

	//Material Update
	this->_info._materialInfo.ambient = Vector3::zero;
	this->_info._materialInfo.diffuse = Vector4(Vector3::one, 1.0f);
	this->_info._materialInfo.specular = Vector4(Vector3::zero, 0.0f);
	this->_info._materialInfo.emissive = Vector3(Vector3::one) / 10.0f;
	this->GetMeshHeap().GetMaterialBufffer().Update(&this->_info._materialInfo, sizeof(RendererMaterial), 0);


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

			_transformBuffer.CreateView(view, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetTransformDescStartIndex()));
		}

		{

			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = this->GetMeshHeap().GetMaterialBufffer().GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(RendererMaterial)));

			this->GetMeshHeap().GetMaterialBufffer().CreateView(view, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetMaterialDescStartIndex()));
		}

		{
			//();
			this->GetMeshHeap().AddTextureRef(TextureManager::GetInstance().GetNullTextureShaderResource().lock());
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = this->GetMeshHeap().GetTexturesRef()[0].lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			this->GetMeshHeap().GetTexturesRef()[0].lock()->CreateView(srvDesc, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetTextureDescStartIndex()));
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
	ID3D12DescriptorHeap* heap[] = { GetMeshHeap().GetHeap().GetPtr() };
	this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

void K3D12::Cube::RegisterToBundle()
{
	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindIndexBufferToBundle();
	BindVertexBufferToBundle();
	BindDescriptorHeaps(_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(1, GetMeshHeap().GetHeap().GetGPUHandle(GetMeshHeap().GetTransformDescStartIndex()));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(2, GetMeshHeap().GetHeap().GetGPUHandle(GetMeshHeap().GetMaterialDescStartIndex()));
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(3, GetMeshHeap().GetHeap().GetGPUHandle(GetMeshHeap().GetTextureDescStartIndex()));

	_bundleList.GetCommandList()->DrawIndexedInstanced(static_cast<unsigned int>(_indexList.size()), 1, 0, 0, 0);

	_bundleList.GetCommandList()->Close();
}

void K3D12::Cube::Update()
{
	GameObject::UpdateTransformBuffer();
}

void K3D12::Cube::AttachTexture(std::string pathName)
{
	//_info.AttachTexture(pathName, _heap->GetCPUHandle(_textureStartPoint));
	GetMeshHeap().GetHeap().Discard();
	GetMeshHeap().GetHeap().Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
	GetMeshHeap().GetHeap().SetName("CubeHeap");
	//ViewCreation
	{

		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _transformBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

			_transformBuffer.CreateView(view, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetTransformDescStartIndex()));
		}

		{

			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = this->GetMeshHeap().GetMaterialBufffer().GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(RendererMaterial)));

			this->GetMeshHeap().GetMaterialBufffer().CreateView(view, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetMaterialDescStartIndex()));
		}

		{
			//();
			this->GetMeshHeap().AddTextureRef(TextureManager::GetInstance().GetSpriteShaderResource(pathName).lock());
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = this->GetMeshHeap().GetTexturesRef()[0].lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			this->GetMeshHeap().GetTexturesRef()[0].lock()->CreateView(srvDesc, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetTextureDescStartIndex()));
		}
	}
	//_bundleList.ResetAllocator();
	//_bundleList.ResetCommandList();

	//RegisterToBundle();
}

void K3D12::Cube::SetUV(float compressionRatio)
{
	for (unsigned int surface = 0; surface < _vertexes.size();surface+=4) {
		_vertexes[surface + 1].texCoord = Vector2(compressionRatio,0.0f);
		_vertexes[surface + 2].texCoord = Vector2(0.0f, compressionRatio);
		_vertexes[surface + 3].texCoord = Vector2(compressionRatio, compressionRatio);
	}
	this->GetMeshBuffer().GetVBO().Map(0,nullptr);
	this->GetMeshBuffer().GetVBO().Update(&this->_vertexes[0], this->_vertexes.size() * sizeof(Vertex3D), 0);
	this->GetMeshBuffer().GetVBO().Unmap(0,nullptr);
}
