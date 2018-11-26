#include "GPUParticle.h"
#include "../System/D3D12System.h"
#include "../Util/Utility.h"
#include "../../Engine/ShaderComponent/ShaderCluster.h"
#include "../CommandContext/GraphicsContextLibrary.h"


#define THREAD_NUM_X 16
#define TEST 1


unsigned int K3D12::GPUParticle::INSTANCE_MAX = 1024 * 2048;
unsigned int K3D12::GPUParticle::SPAWN_MAX = 256 * THREAD_NUM_X;

K3D12::GPUParticle::GPUParticle()
{

}


K3D12::GPUParticle::~GPUParticle()
{
	_reservedSlotsBuffer.Discard();

	_instancePropatiesBuffer.Discard();

	_instanceDrawBuffer.Discard();

	_instanceCountBuffer.Discard();

	_drawArgBuffer.Discard();

	_drawArgCopyBuffer.Discard();

	_initDescriptorHeap.Discard();

	_descriptorHeap.Discard();

	_drawDescriptorHeap.Discard();

	_commandSignature.Discard();

	_spawnData.Discard();

	_sceneConstantBuffer.Discard();

	_wvpMatBuffer.Discard();
}


void K3D12::GPUParticle::Create(int particleMax, int emitNum)
{
	PARTICLE_MAX = (Util::Align(particleMax, THREAD_NUM_X));
	EMIT_NUM = (Util::Align(emitNum, THREAD_NUM_X));

	_spawanDataCount = 0;
	_gameTime = 0.0f;

	CreateBuffers();
	CreateRootSignature();
	CreateDescriptorHeap();
	CreatePipelineState();

	//本初期化
	D3D12_ROOT_PARAMETER param[1] = {};
	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].NumDescriptors = 1;
	range[0].RegisterSpace = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[0].BaseShaderRegister = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

	param[0].DescriptorTable.pDescriptorRanges = range;
	param[0].DescriptorTable.NumDescriptorRanges = 1;
	param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

	D3D12_ROOT_SIGNATURE_DESC sigDesc;
	sigDesc.NumParameters = _countof(param);
	sigDesc.NumStaticSamplers = 0;
	sigDesc.pParameters = param;
	sigDesc.pStaticSamplers = nullptr;
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;

	GraphicsContextLibrary::GetInstance().CreateRootSignature("ParticleInitCSRootSignature", &sigDesc);


	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	{
		ShaderCluster cs = {};
		cs.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE, "./Engine/Shader/ParticleInitCS.hlsl", "InitReservedSlots", "cs_5_0");
		psoDesc.CS = cs.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE);
		psoDesc.pRootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature("ParticleInitCSRootSignature")->GetSignature().Get();
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;;
		psoDesc.NodeMask = 0;
	}
	GraphicsContextLibrary::GetInstance().CreatePSO("ParticleInitPSO", psoDesc);

	//イニシャライズ用デスクリプタヒープ初期化
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		_initDescriptorHeap.Create(&desc);
	}


	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = INSTANCE_MAX;
	uavDesc.Buffer.StructureByteStride = sizeof(unsigned int);
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	_reservedSlotsBuffer.CreateView(&uavDesc, _initDescriptorHeap.GetCPUHandle(0), &_instanceCountBuffer);

	{
		ID3D12DescriptorHeap* ppHeaps[] = { _initDescriptorHeap.GetHeap().Get() };
		auto cmd = K3D12::D3D12System::GetComputeCommandList();

		cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("ParticleInitPSO")->GetPSO().Get());
		cmd->GetCommandList()->SetComputeRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("ParticleInitCSRootSignature")->GetSignature().Get());
		cmd->GetCommandList()->SetDescriptorHeaps(1, ppHeaps);
		cmd->GetCommandList()->SetComputeRootDescriptorTable(0, _initDescriptorHeap.GetGPUHandle(0));

		unsigned int threadNum = (INSTANCE_MAX >> 10);
		cmd->GetCommandList()->Dispatch(threadNum, 1, 1);
		cmd->GetCommandList()->Close();

		//ここで初期化を回す
		{
			ID3D12CommandList* lists[] = { cmd->GetCommandList().Get() };
			K3D12::D3D12System::GetMasterComputeQueue().GetQueue()->ExecuteCommandLists(1, lists);
			K3D12::D3D12System::GetMasterComputeQueue().Wait();
			cmd->ResetAllocator();
			cmd->ResetCommandList();
		}
	}
}

void K3D12::GPUParticle::Run(float deltaTime)
{
	this->_deltaTime = deltaTime;
	this->_gameTime += deltaTime;
}

void K3D12::GPUParticle::Draw()
{
	_sceneConstantBuffer.Data()->deltaTime = _deltaTime;
	_sceneConstantBuffer.Data()->gameTime = _gameTime;
	_sceneConstantBuffer.Data()->spawnerCount = _spawanDataCount;

	{
		Matrix view = K3D12::GetCamera().GetView();
		Matrix proj = K3D12::GetCamera().GetProjection();
		Matrix vp = (view * proj);
		_wvpMatBuffer.CopyData(0, vp);
	}
	// 描画数を０から始める
	{
		D3D12_SUBRESOURCE_DATA subresource = {};
		DrawArgs args = {};
		args.VertexCountPerInstance = 2;
		subresource.pData = &args;
		subresource.RowPitch = sizeof(args);
		subresource.SlicePitch = subresource.RowPitch;

#ifndef TEST 
		{
			K3D12::D3D12System::CreateCommandList("ResourceUpdaterList", 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
			auto ucmd = K3D12::D3D12System::GetCommandList("ResourceUpdaterList");
			{
				//クリア
				_drawArgBuffer.ResourceTransition(ucmd, D3D12_RESOURCE_STATE_COPY_DEST);
				_drawArgCopyBuffer.ResourceTransition(ucmd, D3D12_RESOURCE_STATE_COPY_SOURCE);

				//リソースアップデート
				UpdateSubresources<1>(ucmd->GetCommandList().Get(), _drawArgBuffer.GetResource(), _drawArgCopyBuffer.GetResource(), 0, 0, 1, &subresource);
				_drawArgBuffer.ResourceTransition(ucmd, D3D12_RESOURCE_STATE_GENERIC_READ);
				_drawArgCopyBuffer.ResourceTransition(ucmd, D3D12_RESOURCE_STATE_GENERIC_READ);
			}
			//ここでアップデートを回す
			{
				ucmd->CloseCommandList();
				ID3D12CommandList* lists[] = { ucmd->GetCommandList().Get() };
				K3D12::D3D12System::GetMasterCommandQueue().GetQueue()->ExecuteCommandLists(1, lists);
				K3D12::D3D12System::GetMasterCommandQueue().Wait();
				ucmd->ResetAllocator();
				ucmd->ResetCommandList();
			}

		}
		{
			auto cmd = K3D12::D3D12System::GetComputeCommandList();

			cmd->GetCommandList()->SetComputeRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleCSRootSignature")->GetSignature().Get());

			//でスクリプタヒープセット
			ID3D12DescriptorHeap* ppHeap[] = { _descriptorHeap.GetHeap().Get() };
			cmd->GetCommandList()->SetDescriptorHeaps(1, ppHeap);

			cmd->GetCommandList()->SetComputeRootDescriptorTable(0, _descriptorHeap.GetGPUHandle(0));

			if (_spawanDataCount > 0) {

				cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("ParticleSpawnPSO")->GetPSO().Get());
				cmd->GetCommandList()->Dispatch(THREAD_NUM_X, 1, 1);

			}

			cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("ParticleUpdatePSO")->GetPSO().Get());
			cmd->GetCommandList()->Dispatch(INSTANCE_MAX >> 10, 1, 1);
			//ここで実行を回す
			{
				cmd->CloseCommandList();
				ID3D12CommandList* lists[] = { cmd->GetCommandList().Get() };
				K3D12::D3D12System::GetMasterComputeQueue().GetQueue()->ExecuteCommandLists(1, lists);
				K3D12::D3D12System::GetMasterComputeQueue().Wait();
				cmd->ResetAllocator();
				cmd->ResetCommandList();
			}
		}
		//描画ステップ
		{
			auto cmd = K3D12::D3D12System::GetMasterCommandList();
			//メインレンダーターゲットのセット
			cmd->GetCommandList()->ClearDepthStencilView(K3D12::GetCamera().GetDepthStencil().GetDSVHeapPtr()->GetCPUHandle(0),
				D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 1, &K3D12::D3D12System::GetInstance().GetWindow().GetScissorRect());
			K3D12::SetMainRenderTarget(cmd, &K3D12::GetCamera().GetDepthStencil().GetDSVHeapPtr()->GetCPUHandle(0));
			cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("DrawPSO")->GetPSO().Get());
			cmd->GetCommandList()->SetGraphicsRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleDrawRootSignature")->GetSignature().Get());
			ID3D12DescriptorHeap* ppHeap[] = { _drawDescriptorHeap.GetHeap().Get() };

			ppHeap[0] = _drawDescriptorHeap.GetHeap().Get();
			cmd->GetCommandList()->SetDescriptorHeaps(1, ppHeap);
			cmd->GetCommandList()->SetGraphicsRootDescriptorTable(0, _drawDescriptorHeap.GetGPUHandle(0));
			cmd->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			cmd->GetCommandList()->ExecuteIndirect(_commandSignature.GetSignature().Get(), 1, _drawArgBuffer.GetResource(), sizeof(DrawArgs::pad), nullptr, 0);
		}
	}
#else
	{
		auto cmd = K3D12::D3D12System::GetMasterCommandList();

		{

			//クリア
			_drawArgBuffer.ResourceTransition(cmd, D3D12_RESOURCE_STATE_COPY_DEST);
			_drawArgCopyBuffer.ResourceTransition(cmd, D3D12_RESOURCE_STATE_COPY_SOURCE);

			//リソースアップデート
			UpdateSubresources<1>(cmd->GetCommandList().Get(), _drawArgBuffer.GetResource(), _drawArgCopyBuffer.GetResource(), 0, 0, 1, &subresource);
			_drawArgBuffer.ResourceTransition(cmd, D3D12_RESOURCE_STATE_GENERIC_READ);
			_drawArgCopyBuffer.ResourceTransition(cmd, D3D12_RESOURCE_STATE_GENERIC_READ);


		}
		{

			cmd->GetCommandList()->SetComputeRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleCSRootSignature")->GetSignature().Get());

			//でスクリプタヒープセット
			ID3D12DescriptorHeap* ppHeap[] = { _descriptorHeap.GetHeap().Get() };
			cmd->GetCommandList()->SetDescriptorHeaps(1, ppHeap);

			cmd->GetCommandList()->SetComputeRootDescriptorTable(0, _descriptorHeap.GetGPUHandle(0));

			if (_spawanDataCount > 0) {

				cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("ParticleSpawnPSO")->GetPSO().Get());
				cmd->GetCommandList()->Dispatch(THREAD_NUM_X, 1, 1);

			}

			cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("ParticleUpdatePSO")->GetPSO().Get());
			cmd->GetCommandList()->Dispatch(INSTANCE_MAX >> 10, 1, 1);
		}
		//描画ステップ
		{
			//メインレンダーターゲットのセット
			cmd->GetCommandList()->ClearDepthStencilView(K3D12::GetCamera().GetDepthStencil().GetDSVHeapPtr()->GetCPUHandle(0),
				D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 1, &K3D12::D3D12System::GetInstance().GetWindow().GetScissorRect());
			K3D12::SetMainRenderTarget(cmd, &K3D12::GetCamera().GetDepthStencil().GetDSVHeapPtr()->GetCPUHandle(0));
			cmd->GetCommandList()->SetPipelineState(GraphicsContextLibrary::GetInstance().GetPSO("DrawPSO")->GetPSO().Get());
			cmd->GetCommandList()->SetGraphicsRootSignature(GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleDrawRootSignature")->GetSignature().Get());
			ID3D12DescriptorHeap* ppHeap[] = { _drawDescriptorHeap.GetHeap().Get() };

			ppHeap[0] = _drawDescriptorHeap.GetHeap().Get();
			cmd->GetCommandList()->SetDescriptorHeaps(1, ppHeap);
			cmd->GetCommandList()->SetGraphicsRootDescriptorTable(0, _drawDescriptorHeap.GetGPUHandle(0));
			cmd->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			cmd->GetCommandList()->ExecuteIndirect(_commandSignature.GetSignature().Get(), 1, _drawArgBuffer.GetResource(), sizeof(DrawArgs::pad), nullptr, 0);
		}
	}

#endif
	_spawanDataCount = 0;

	}
}

void K3D12::GPUParticle::Spawn(int num, Vector3 pos, float speedMag, float lengthMag, float reductionRate)
{
	//遅延によるスポーンミスのカバー
	num = min(num, static_cast<int> (INSTANCE_MAX * _deltaTime - 1000));

	for (int i = 0; i < num && (unsigned int)_spawanDataCount < SPAWN_MAX; i++) {
		auto& spawn = _spawnData.Data()[i];
		spawn.position = Vector4(pos, 1.0f);
		spawn.colorSamplingV = Util::frand()*0.5f + 0.5f;
		auto th = Util::frand() * F_2PI;
		spawn.forward = Vector3(sinf(th), cosf(th), 0.0).Normalize();
		spawn.initialSpeedFactor = Util::frand()*1.25f + 0.25f;
		spawn.speedMag = speedMag;
		spawn.lengthMag = lengthMag;
		spawn.reductionRate = reductionRate;

		_spawanDataCount++;
	}
}

void K3D12::GPUParticle::CreateBuffers()
{
	_reservedSlotsBuffer.Create(sizeof(unsigned int), INSTANCE_MAX, nullptr);
	_instancePropatiesBuffer.Create(sizeof(InstanceData), INSTANCE_MAX, nullptr);
	_instanceDrawBuffer.Create(sizeof(InstanceDrawData), INSTANCE_MAX, nullptr);

	{
		DrawArgs args = {};
		args.pad[0] = 2;
		args.VertexCountPerInstance = 2;
		args.InstanceCount = 2;
		args.StartVertexLocation = 2;
		args.StartInstanceLocation = 2;

		_drawArgBuffer.Create(sizeof(DrawArgs), 1, &args);
		//
		_drawArgCopyBuffer.Create(sizeof(DrawArgs), 1, nullptr);
		//
		_instanceCountBuffer.Create(sizeof(args), 1, &args);

		_spawnData.Create(INSTANCE_MAX);
		_sceneConstantBuffer.Create(1, true);

		_wvpMatBuffer.Create(1, true);

	}

}

void K3D12::GPUParticle::CreateRootSignature()
{
	D3D12_ROOT_PARAMETER param[1] = {};
	D3D12_DESCRIPTOR_RANGE range[3] = {};

	range[0].NumDescriptors = 3;
	range[0].RegisterSpace = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[0].BaseShaderRegister = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

	range[1].NumDescriptors = 1;
	range[1].RegisterSpace = 0;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[1].BaseShaderRegister = 0;
	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

	range[2].NumDescriptors = 1;
	range[2].RegisterSpace = 0;
	range[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[2].BaseShaderRegister = 0;
	range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[0].DescriptorTable.NumDescriptorRanges = 3;
	param[0].DescriptorTable.pDescriptorRanges = range;

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
	sampler.Filter = D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.MaxAnisotropy = 16;
	sampler.ShaderRegister = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	sampler.MipLODBias = 0.0f;
	sampler.MaxLOD = 3.402823466e+38f;
	sampler.RegisterSpace = 0;

	D3D12_ROOT_SIGNATURE_DESC sigDesc;
	sigDesc.NumParameters = _countof(param);
	sigDesc.NumStaticSamplers = 1;
	sigDesc.pParameters = param;
	sigDesc.pStaticSamplers = &sampler;
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;

	GraphicsContextLibrary::GetInstance().CreateRootSignature("GPUParticleCSRootSignature", &sigDesc);

	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
	param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[0].DescriptorTable.NumDescriptorRanges = 2;
	param[0].DescriptorTable.pDescriptorRanges = range;

	range[0].NumDescriptors = 1;
	range[0].RegisterSpace = 0;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[0].BaseShaderRegister = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

	range[1].NumDescriptors = 1;
	range[1].RegisterSpace = 0;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[1].BaseShaderRegister = 0;
	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	GraphicsContextLibrary::GetInstance().CreateRootSignature("GPUParticleDrawRootSignature", &sigDesc);

	//コマンドシグネチャ作るよ
	{
		// コマンドシグネチャもここで作る
		D3D12_COMMAND_SIGNATURE_DESC commandDesc = {};
		D3D12_INDIRECT_ARGUMENT_DESC argumentDescs[1] = {};
		argumentDescs[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
		commandDesc.ByteStride = sizeof(IndirectCommand);
		commandDesc.NumArgumentDescs = _countof(argumentDescs);
		commandDesc.pArgumentDescs = argumentDescs;

		_commandSignature.Create(commandDesc);
	}

}

void K3D12::GPUParticle::CreateDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = 5;

	_descriptorHeap.Create(&desc);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(unsigned int);
	uavDesc.Buffer.NumElements = INSTANCE_MAX;

	_reservedSlotsBuffer.CreateView(&uavDesc, _descriptorHeap.GetCPUHandle(0), &_instanceCountBuffer);

	uavDesc.Buffer.StructureByteStride = sizeof(InstanceDrawData);
	uavDesc.Buffer.CounterOffsetInBytes = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;

	_instanceDrawBuffer.CreateView(&uavDesc, _descriptorHeap.GetCPUHandle(1), &_drawArgBuffer);

	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.StructureByteStride = sizeof(InstanceData);

	_instancePropatiesBuffer.CreateView(&uavDesc, _descriptorHeap.GetCPUHandle(2));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = _sceneConstantBuffer.GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = _sceneConstantBuffer.ElementByteSize();

	_sceneConstantBuffer.CreateView(cbvDesc, _descriptorHeap.GetCPUHandle(3));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INSTANCE_MAX;
	srvDesc.Buffer.StructureByteStride = _spawnData.ElementByteSize();
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	_spawnData.CreateView(srvDesc, _descriptorHeap.GetCPUHandle(4));

	//描画時のデスクリプタヒープ

	desc.NumDescriptors = 2;

	_drawDescriptorHeap.Create(&desc);

	cbvDesc.BufferLocation = _wvpMatBuffer.GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = _wvpMatBuffer.ElementByteSize();

	_wvpMatBuffer.CreateView(cbvDesc, _drawDescriptorHeap.GetCPUHandle(0));

	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INSTANCE_MAX;
	srvDesc.Buffer.StructureByteStride = sizeof(InstanceDrawData);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	_instanceDrawBuffer.CreateView(&srvDesc, _drawDescriptorHeap.GetCPUHandle(1));
}

void K3D12::GPUParticle::CreatePipelineState()
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC compPSO = {};
	//スポーン
	{
		ShaderCluster computeShader;
		computeShader.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE, "./Engine/Shader/ParticleSpawnCS.hlsl", "SpawnParticles", "cs_5_0", "./Engine/Shader/");
		compPSO.pRootSignature;
		compPSO.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
		compPSO.CS = computeShader.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE);
		compPSO.pRootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleCSRootSignature")->GetSignature().Get();
		GraphicsContextLibrary::GetInstance().CreatePSO("ParticleSpawnPSO", compPSO);

	}
	//アップデート
	{
		ShaderCluster computeShader;
		computeShader.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE, "./Engine/Shader/ParticleUpdateCS.hlsl", "UpdateParticles", "cs_5_0", "./Engine/Shader/");
		compPSO.pRootSignature;
		compPSO.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
		compPSO.CS = computeShader.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_COMPUTE);
		compPSO.pRootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleCSRootSignature")->GetSignature().Get();
		GraphicsContextLibrary::GetInstance().CreatePSO("ParticleUpdatePSO", compPSO);

	}
	//描画用
	{

		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));

		desc.InputLayout = { nullptr, 0 };
		desc.pRootSignature = GraphicsContextLibrary::GetInstance().GetRootSignature("GPUParticleDrawRootSignature")->GetSignature().Get();;
		{
			ShaderCluster shader;
			shader.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_VERTEX, "./Engine/Shader/GPUParticleVS.hlsl", "VS", "vs_5_0", "");
			desc.VS = shader.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_VERTEX);
		}
		{
			ShaderCluster shader;
			shader.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_GEOMETRY, "./Engine/Shader/ColorParticleGS.hlsl", "GS", "gs_5_0", "./Engine/Shader/");
			desc.GS = shader.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_VERTEX);
		}
		{
			ShaderCluster shader;
			shader.CompileShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_PIXEL, "./Engine/Shader/ColorParticlePS.hlsl", "PS", "ps_5_0", "./Engine/Shader/");
			desc.PS = shader.GetShader(ShaderCluster::SHADER_TYPE::SHADER_TYPE_PIXEL);

		}


		desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		desc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		desc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		desc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
		desc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		desc.SampleMask = UINT_MAX;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		GraphicsContextLibrary::GetInstance().CreatePSO("DrawPSO", desc);

	}

}

