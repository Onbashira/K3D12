#define TestComputeRootSignature   "RootFlags(0),"\
                                   "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                     "visibility = SHADER_VISIBILITY_ALL),"\

struct TestVertex
{
    uint indexBuffer;
    float3 vertex;
};

RWStructuredBuffer<TestVertex> VertexBuffer : register(u0);

[RootSignature(TestComputeRootSignature)]
[numthreads(10, 1, 1)]
void CSMain(uint3 groupeID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint size = 0;
    uint stride = 0;
    VertexBuffer.GetDimensions(size, stride);
    for (uint i = 0; i < size;++i)
    {
        TestVertex a = VertexBuffer[i];
        a.indexBuffer = i;
        a.vertex += float3(1.0f, 1.0f, 1.0f);
        VertexBuffer[i] = a;
    }
    
    GroupMemoryBarrierWithGroupSync();

    VertexBuffer.IncrementCounter();

}


void DFT()
{

}

void FFT()
{

}

void InversFFT()
{

}

