
#include "ParticleInstanceData.h"
#include "../System/D3D12System.h"
#include "../CommandContext/GraphicsCommandList.h"
#include "../TextureComponent/TextureManager.h"

constexpr unsigned int PARTICLE_MAX_NUM = 1024 * 100;


K3D12::ParticleInstanceData::ParticleInstanceData()
{
	Discard();
}

K3D12::ParticleInstanceData::~ParticleInstanceData()
{
}

void K3D12::ParticleInstanceData::InitHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = OFFSET_PARTICLE_OFFSET_MAX;
	desc.NodeMask = 0;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	this->_particleHeap.Create(&desc);
}

void K3D12::ParticleInstanceData::InitView()
{
	//CBV作成
	Camera& cam = K3D12::GetCamera();
	{
	}
	//SRV作成
	{
		_particleCreateData.CreateView(&_particleCreateData.GetSRVDesc(), _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_PARTICLE_CREATE_DATA));

	}
	//UAV作成
	{
		_particleData.CreateView(&_particleData.GetUAVDesc(), _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_PARTICLE_DATA));
		_drawArgumentData.CreateView(&_drawArgumentData.GetUAVDesc(), _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_DRAW_ARGUMENT_DATA));
		_reservedIndexData.CreateView(&_reservedIndexData.GetUAVDesc(), _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_RESERVE_SLOT_APPEND_DATA));
		_reservedIndexData.CreateView(&_reservedIndexData.GetUAVDesc(), _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_RESERVE_SLOT_CONSUME_DATA));

	}

}

void K3D12::ParticleInstanceData::LoadParticleTexture(std::string path)
{
	K3D12::TextureLoader::GetInstance().LoadSpriteTexture(path, _particleHeap.GetCPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_SPRITE));
}


void K3D12::ParticleInstanceData::Create(unsigned int spawnCountParFrame, unsigned int commandCountParFrame)
{

	this->_particleConstants._commandCount = commandCountParFrame;
	this->_particleConstants._spawnCount = spawnCountParFrame;
	this->_liveParticleNum = 0;

	this->_drawArgumentData.Create(sizeof(ParticleIndirectCommand), commandCountParFrame, nullptr);
	this->_particleCreateData.Create(sizeof(ParticleData), spawnCountParFrame, nullptr);
	this->_reservedIndexData.Create(sizeof(unsigned int), spawnCountParFrame, nullptr);
	this->_particleData.Create(sizeof(ParticleData), PARTICLE_MAX_NUM, nullptr);
	//Drawオーグメントを追加
	{
		D3D12_INDIRECT_ARGUMENT_DESC desc;
		desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
		desc.VertexBuffer.Slot = 0;//頂点数か？要調査 -> IASetVertexBufferのStartSlotっぽい？0でいい？
		AddDrawDesc(desc);
		desc = {};
		desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
		desc.ConstantBufferView.RootParameterIndex = 0;
		AddDrawDesc(desc);
		desc = {};
		desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
		desc.ShaderResourceView.RootParameterIndex = 1;
		AddDrawDesc(desc);
		desc = {};
		desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
	}
	_vertexBufferView.BufferLocation = _particleData.GetResource()->GetGPUVirtualAddress();
	_vertexBufferView.SizeInBytes = sizeof(ParticleData) * PARTICLE_MAX_NUM;
	_vertexBufferView.StrideInBytes = sizeof(ParticleData);

	//パーティクル初期化用データの初期化
	{

		this->_particleInitBuffer.resize(spawnCountParFrame);
		float  div = F_2PI / spawnCountParFrame;
		for (int i = 0; i < spawnCountParFrame; i++) {
			_particleInitBuffer[i].angle = div * i;
			_particleInitBuffer[i].liveTime = 1.0f;
			_particleInitBuffer[i].size = 2.0f;
			_particleInitBuffer[i].pos = Vector4(cosf(div*i), sinf(div*i), 0.0f, 1.0f);
			_particleInitBuffer[i].velocity = Vector2(_particleInitBuffer[i].pos.x, _particleInitBuffer[i].pos.y).Normalize();
		}

		_particleCreateData.WriteToBuffer(spawnCountParFrame, sizeof(ParticleData), &_particleInitBuffer[0]);
	}
	//パーティクル描画コマンドの初期化
	{
		this->_indirectDrawBuffer.resize(spawnCountParFrame);

		D3D12_DRAW_ARGUMENTS argument;
		argument.InstanceCount = 1;
		argument.StartInstanceLocation = 0;
		argument.StartVertexLocation = 0;
		argument.VertexCountPerInstance = 1;

		unsigned int stride = 0, size = 0;
		stride = size = sizeof(ParticleData);
		D3D12_GPU_VIRTUAL_ADDRESS vertexBufferAddressBase = _particleData.GetResource()->GetGPUVirtualAddress();
		unsigned long long VertexAddressOffset = sizeof(ParticleData);
		ParticleIndirectCommand command;
		command.drawArguments = argument;
		command.vertexBufferGPUAddress = vertexBufferAddressBase;
		command.vertexBufferSize = size;
		command.vertexBufferStride = stride;
		command.cameraConstantData = K3D12::GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress();
		command.shaderResourceGPUAddress = _particleHeap.GetGPUHandle(PARTICLE_HEAP_DESCRIPTOR_OFFSET::OFFSET_SPRITE);

		for (int i = 0; i < commandCountParFrame; ++i) {

		}

		_drawArgumentData.WriteToBuffer(commandCountParFrame, sizeof(ParticleIndirectCommand), &_particleInitBuffer[0]);

	}

}

void K3D12::ParticleInstanceData::CreateCommandSignature(std::weak_ptr<K3D12::RootSignature> rs)
{

}

void K3D12::ParticleInstanceData::AddDrawDesc(D3D12_INDIRECT_ARGUMENT_DESC indirectPropaties)
{
	this->_drawDescs.push_back(indirectPropaties);
}

void K3D12::ParticleInstanceData::SetDescriptorHeap(std::weak_ptr<GraphicsCommandList> computeCommandList)
{
	ID3D12DescriptorHeap* heap[] = { this->_particleHeap.GetHeap().Get() };
	computeCommandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
}

void K3D12::ParticleInstanceData::SetVertexBuffer(std::weak_ptr<GraphicsCommandList> graphicsCommandList)
{
	graphicsCommandList.lock()->GetCommandList()->IASetVertexBuffers(0, 1, &_vertexBufferView);
}

void K3D12::ParticleInstanceData::InitParticleIndirectCommandBuffer(unsigned int size, unsigned int stride, void * pBuffer)
{
}

void K3D12::ParticleInstanceData::InitParticleInitializeBuffer(unsigned int size, unsigned int stride, void * pBuffer)
{
}

void K3D12::ParticleInstanceData::Discard()
{
	this->_commandSignature.Discard();
	this->_drawDescs.clear();
	this->_drawDescs.shrink_to_fit();
	this->_particleCreateData.Discard();
	this->_particleData.Discard();
	this->_reservedIndexData.Discard();
	this->_particleHeap.Discard();
}
