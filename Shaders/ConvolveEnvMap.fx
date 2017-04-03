#include "ConvolveEnvMap.data.fx"
#include "MonteCarlo.inc.fx"

#define PI_OVER_2 (PI / 2.0)
#define PI_OVER_4 (PI / 4.0)

[numthreads(NUM_THREADS, NUM_THREADS, 1)]
void main(uint3 threadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    float2 angles = ((threadID.xy / (Resolution - 1.0)) * PI_OVER_2 - PI_OVER_4) * float2(1.0, -1.0);
    float2 sines, cosines;
    sincos(angles, sines, cosines);
    float3 proj = cosines.x * Normal + sines.x * Right;
    float3 forward = cosines.y * proj + sines.y * Up;
    //float3 test = float3(sines.y * sines.x, cosines.y, sines.x * cosines.y);
    //forward = Right * test.x + Up * test.y + Normal * test.z;
    uint3 arrayCoords = uint3(threadID.xy, Face);
    IrradianceCubeMapUAV[arrayCoords] = float4(forward.zzz, 1.0);//EnvironmentMap.SampleLevel(EnvMapSampler, forward, 0);
}