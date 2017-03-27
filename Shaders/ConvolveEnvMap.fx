#include "ConvolveEnvMap.data.fx"

[numthreads(NUM_THREADS, NUM_THREADS, 1)]
void main(uint3 threadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint3 arrayCoords = uint3(threadID.xy, Face);
    IrradianceCubeMapUAV[arrayCoords] = float4(1, 0, 1, 1);
}