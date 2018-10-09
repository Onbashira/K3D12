#include "Sprite.h"
#include "Math.h"
#include "../Primitive/PrimitiveComponent.h"
#include "../../TextureComponent/TextureManager.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../Resource/ShaderResource.h"
#include "../../Font/FontData.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../../Util/Math.h"
#include "../../CommandContext/CommandListLibrary.h"



K3D12::Sprite::Sprite() : _height(0), _width(0)
{
	Initializer();
}


K3D12::Sprite::~Sprite()
{
}

void K3D12::Sprite::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(0, K3D12::GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	//ID3D12DescriptorHeap* heap[] = { GetMeshHeap().GetHeap().GetPtr() };
	//this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	BindDescriptorHeaps(_commandList);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

void K3D12::Sprite::DrawString(std::weak_ptr<FontData> font, std::string str)
{
	//HDC hdc = GetDC(NULL);
	//HFONT oldFont = (HFONT)SelectObject(hdc, font.lock()->GetFontHandle());

	//unsigned int code = 0;

	////フォントビットマップの取得
	//TEXTMETRIC textMetric;
	//GetTextMetrics(hdc, &textMetric);
	//GLYPHMETRICS glyphMetrics;

	//const MAT2 mat = { {0,1},{0,0},{0,0},{1,0} };
	//DWORD size = GetGlyphOutline(
	//	hdc,
	//	code,
	//	GGO_GRAY4_BITMAP,
	//	&glyphMetrics,
	//	0,
	//	NULL,
	//	&mat
	//);

	//BYTE* ptr = new BYTE[size];
	//for (int codeNum = 0; codeNum < str.size(); ++codeNum) {

	//	unsigned int code = 0;

	//	//フォントビットマップの取得
	//	TEXTMETRIC textMetric;
	//	GetTextMetrics(hdc, &textMetric);
	//	GLYPHMETRICS glyphMetrics;

	//	const MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 1,0 } };
	//	DWORD size = GetGlyphOutline(
	//		hdc,
	//		code,
	//		GGO_GRAY4_BITMAP,
	//		&glyphMetrics,
	//		0,
	//		NULL,
	//		&mat
	//	);

	//	GetGlyphOutline(
	//		hdc,
	//		code,
	//		GGO_GRAY4_BITMAP,
	//		&glyphMetrics,
	//		size,
	//		ptr,
	//		&mat
	//	);

	//	SelectObject(hdc, oldFont);
	//	ReleaseDC(NULL, hdc);

	//	D3D12_RESOURCE_DESC desc;
	//	memset(&desc, 0, sizeof(D3D12_RESOURCE_DESC));


	//	ShaderResource resource;
	//	{
	//		D3D12_HEAP_PROPERTIES props = {};
	//		props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	//		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//		props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	//		props.CreationNodeMask = 0;
	//		props.VisibleNodeMask = 0;

	//		D3D12_RESOURCE_DESC resDesc = {};
	//		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//		desc.Width = glyphMetrics.gmCellIncX;
	//		desc.Height = textMetric.tmHeight;
	//		resDesc.DepthOrArraySize = 1;
	//		resDesc.MipLevels = 1;
	//		resDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	//		resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//		resDesc.SampleDesc.Count = 1;
	//		resDesc.SampleDesc.Quality = 0;
	//		resDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	//		resource.Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	//	}
	//	UCHAR* buffer;
	//	resource.Map(0, nullptr);
	//	buffer = resource.GetMappedPointer();
	//	int offsetX = glyphMetrics.gmptGlyphOrigin.x;
	//	int offsetY = textMetric.tmAscent - glyphMetrics.gmptGlyphOrigin.y;
	//	int bmpWidth = glyphMetrics.gmBlackBoxX + (4 - (glyphMetrics.gmBlackBoxX % 4)) % 4;
	//	int bmpHeight = glyphMetrics.gmBlackBoxY;
	//	int level = 17; // アルファ値の階層のレベル
	//	int x = 0, y = 0;
	//	DWORD alpha = 0, color = 0;

	//	memset(buffer, 0, textMetric.tmHeight * resource->GetDesc().Width);
	//	for (y = offsetY; y < offsetY + bmpHeight; ++y) {
	//		for (x = offsetX; x < offsetX + bmpWidth; ++x) {
	//			alpha = (255 * ptr[x - offsetX + bmpWidth * (y - offsetY)]) / (level - 1);
	//			color = 0x00ffffff | (alpha << 24);
	//			std::memcpy(buffer + resource->GetDesc().Width * y + 4 * x, &color, sizeof(DWORD));
	//		}
	//	}

	//	resource.Unmap(0, nullptr);
	//}
}

void K3D12::Sprite::Initializer()
{
	this->GetMeshHeap().SetTransformDescStartIndex(0);
	this->GetMeshHeap().SetMaterialDescStartIndex();
	this->GetMeshHeap().SetTextureDescStartIndex();

	unsigned int sufaceCount = 1;
	unsigned int planeVertex = 4;
	Vertex3D plane[4];

	for (unsigned int i = 0; i < planeVertex; ++i) {
		plane[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f), 0.0f, -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f));
		plane[i].normal = Vector3::back;
		plane[i].texCoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
		this->_vertexes.push_back(plane[i]);
	}
	unsigned int planeList[] = { 0,1,2,1,3,2 };
	for (unsigned int listIndex = 0; listIndex < sufaceCount; ++listIndex) {
		this->_indexList.push_back(planeList[listIndex]);
	}

	// Resource Creation
	{
		this->GetMeshBuffer().GetIBO().Create(_indexList.size() * sizeof(unsigned int), sizeof(unsigned int), &this->_indexList[0]);
		this->GetMeshBuffer().GetIBO().SetName("SpriteIndexBuffer");

		this->GetMeshBuffer().GetVBO().Create(_vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &this->_vertexes[0]);
		this->GetMeshBuffer().GetVBO().SetName("SpriteVertexBuffer;");

		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		this->GetMeshHeap().GetMaterialBufffer().Create(Util::Alignment256Bytes(sizeof(RendererMaterial)));
		this->GetMeshHeap().GetMaterialBufffer().SetName("SpriteMaterialBuffer");
	}

	//MaterialFactor Update
	{
		this->_info.alpha = 1.0f;
		this->_info.color = Vector3::zero;
		this->_info.centerOffset = Vector2(0.0f, 0.0f);
		this->GetMeshHeap().GetMaterialBufffer().Update(&_info, sizeof(SpriteInfo), 0);
	}
	//TransformUpdate
	{
		SetScale(Vector2(1.0f, 1.0f));
		Translate(Vector2(0.0f, 0.0f));
		SetRotation(Quaternion::CreateFromEulerAngles(Vector3::zero));
		GameObject::UpdateTransformBuffer();
	}
	//HeapCreation
	GetMeshHeap().GetHeap().Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
	GetMeshHeap().GetHeap().SetName("SpriteHeap");
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
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			// 寿命切れリンク切れ判定

			this->GetMeshHeap().AddTextureRef(TextureManager::GetInstance().GetNullTextureShaderResource().lock());
			srvDesc.Format = this->GetMeshHeap().GetTexturesRef()[0].lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			this->GetMeshHeap().GetTexturesRef()[0].lock()->CreateView(srvDesc, GetMeshHeap().GetHeap().GetCPUHandle(GetMeshHeap().GetTextureDescStartIndex()));

			_width = static_cast<unsigned int>(this->GetMeshHeap().GetTexturteRef(0).lock()->GetResourceDesc()->Width);
			_height = static_cast<unsigned int>(this->GetMeshHeap().GetTexturteRef(0).lock()->GetResourceDesc()->Height);

		}

		//SetBundleDrawCall
		{
			RegisterToBundle();
		}
	}
}

void K3D12::Sprite::RegisterToBundle()
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

void K3D12::Sprite::AttachTexture(std::string path)
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
			this->GetMeshHeap().AddTextureRef(TextureManager::GetInstance().GetSpriteShaderResource(path).lock());
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
}

void K3D12::Sprite::SetRect(float top, float left, float bottom, float right)
{

	float pos[4] = { left,right,top,bottom };
	for (int i = 0; i < _vertexes.size(); i++) {
		_vertexes[i].texCoord = Vector2(pos[i % 2], pos[(i) % _vertexes.size() / 2 + 2]);
	}

	this->GetMeshBuffer().GetIBO().Update(&_vertexes[0], sizeof(Vertex3D)*_vertexes.size(), 0);

}

void K3D12::Sprite::SetRect(Vector2 topLeft, Vector2 bottomRight)
{

	float pos[4] = { topLeft.x,bottomRight.x,topLeft.y,bottomRight.y };
	for (int i = 0; i < _vertexes.size(); i++) {
		_vertexes[i].texCoord = Vector2(pos[i % 2], pos[(i) % _vertexes.size() / 2 + 2]);
	}

	this->GetMeshBuffer().GetIBO().Update(&_vertexes[0], sizeof(Vertex3D)*_vertexes.size(), 0);
}

void K3D12::Sprite::SetColor(Vector3 color)
{
	this->_info.color = std::move(color);
	this->GetMeshHeap().GetMaterialBufffer().Update(&_info, sizeof(SpriteInfo), 0);
}

void K3D12::Sprite::SetColor(float r, float g, float b)
{
	this->_info.color = std::move(Vector3::Saturate(Vector3(r, g, b)));
	this->GetMeshHeap().GetMaterialBufffer().Update(&_info, sizeof(SpriteInfo), 0);
}

void K3D12::Sprite::SetTransparency(float alpha)
{
	this->_info.alpha = std::move(alpha);

	this->GetMeshHeap().GetMaterialBufffer().Update(&_info, sizeof(SpriteInfo), 0);

}

void K3D12::Sprite::Translate(Vector2 pos)
{
	GameObject::Translate(Vector3(pos.x, pos.y, 0.0f));
	K3D12::GameObject::UpdateTransformBuffer();
}

void K3D12::Sprite::Move(Vector2 velocity)
{
	K3D12::GameObject::Move(Vector3(velocity.x, velocity.y, 0.0f));
	K3D12::GameObject::UpdateTransformBuffer();
}

Vector2 K3D12::Sprite::GetPos()
{
	return Vector2(GameObject::GetPos().x, GameObject::GetPos().y);
}

void K3D12::Sprite::SetScale(Vector2 scale)
{
	K3D12::GameObject::SetScale(Vector3(scale.x, scale.y, 0.0f));
	K3D12::GameObject::UpdateTransformBuffer();
}

Vector2 K3D12::Sprite::GetScale()
{
	return Vector2(GameObject::GetScale().x, GameObject::GetScale().y);
}

void K3D12::Sprite::Rotate(float deg)
{
	K3D12::GameObject::RotationEulerAngles(Vector3(0.0f, deg, 0.0f));
	K3D12::GameObject::UpdateTransformBuffer();
}

void K3D12::Sprite::Rotate(Quaternion quaternion)
{
	K3D12::GameObject::Rotation(quaternion);
}

void K3D12::Sprite::Update()
{
	K3D12::GameObject::UpdateTransformBuffer();
}

void K3D12::Sprite::LateUpdate()
{

}
