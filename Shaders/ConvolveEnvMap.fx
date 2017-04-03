#include "ConvolveEnvMap.data.fx"
#include "MonteCarlo.inc.fx"

[numthreads(NUM_THREADS, NUM_THREADS, 1)]
void main(uint3 threadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    float2 d = ((threadID.xy / Resolution) * 2.0f - 1.0) * float2(1.0, -1.0);
    float2 denom = rcp(sqrt( 1.0 + d * d));
    float2 cosines = 1.0 * denom;
    float2 sines = d * denom;
    float3 proj = cosines.x * Normal + sines.x * Right;
    float3 forward = cosines.y * proj + sines.y * Up;

    uint3 arrayCoords = uint3(threadID.xy, Face);
    IrradianceCubeMapUAV[arrayCoords] = float4(forward, 1.0);//EnvironmentMap.SampleLevel(EnvMapSampler, forward, 0);
}