
AppendStructuredBuffer<uint> reservedSlotsApp : register(u0);

[numthreads(1024, 1, 1)]
void InitReservedSlots(uint index : SV_DispatchThreadID)
{
    reservedSlotsApp.Append(index);
}
