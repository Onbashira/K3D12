#include "Particle.h"
#include "../../Engine/CommandContext/GraphicsContextLibrary.h"


K3D12::Particle::Particle()
{
}


K3D12::Particle::~Particle()
{
}

void K3D12::Particle::Create(EmitterParam param)
{
	this->_emmiterParam = param;

	//DescriptorHeap
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		_constantsHeap.Create(&desc);
	}

	//ConstantBuffe
	{
		_constantBuffer.Create(1, true);
		D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = _constantBuffer.GetResource()->GetGPUVirtualAddress();

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = _constantBuffer.GetResource()->GetGPUVirtualAddress();
		desc.SizeInBytes = _constantBuffer.ElementByteSize();

		_constantBuffer.CreateView(desc, _constantsHeap.GetCPUHandle(0));
	}

	//RootSignature(Test)
	{

		CD3DX12_ROOT_PARAMETER rootParam[1];

		CD3DX12_DESCRIPTOR_RANGE cbvTable;
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		rootParam[0].InitAsDescriptorTable(1, &cbvTable);
		D3D12_ROOT_PARAMETER param;
		
		D3D12_DESCRIPTOR_RANGE range;



		CD3DX12_ROOT_SIGNATURE_DESC desc(1, rootParam, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		GraphicsContextLibrary::GetInstance().CreateRootSignature("Particle", desc.pParameters);
	}

	//Giometry
	{
		_vertexBuffer.Create(param.maxNum, true);
		_vertexBuffer.CopyArray(_vertices.size(), _vertices.data());

	}

	//PSO
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};

		D3D12_INPUT_ELEMENT_DESC layouts[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		desc.InputLayout = { layouts, _countof(layouts) };

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
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

		GraphicsContextLibrary::GetInstance().CreatePSO("Particle", desc);
	}

}

void K3D12::Particle::Run(float deltaTime)
{
}

void K3D12::Particle::Draw()
{
}
