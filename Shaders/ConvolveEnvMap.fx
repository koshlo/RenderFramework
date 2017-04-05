#include "ConvolveEnvMap.data.fx"
#include "MonteCarlo.inc.fx"

#define PI_OVER_2 (PI / 2.0)
#define PI_OVER_4 (PI / 4.0)

static const uint NumSamples = 2000;

[numthreads(NUM_THREADS, NUM_THREADS, 1)]
void main(uint3 threadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    float2 angles = ((threadID.xy / Resolution) * PI_OVER_2 - PI_OVER_4) * float2(1.0, -1.0); 
    float2 sines, cosines;
    sincos(angles, sines, cosines);

    float3 outputNormalTS = float3(cosines.y * sines.x, sines.y * cosines.x, cosines.y * cosines.x);
    float3 outputNormalWS = Right * outputNormalTS.x + Up * outputNormalTS.y + Normal * outputNormalTS.z;

    uint2 random = FrameRandom;

    float3 irradiance = 0;
    for (uint i = 0; i < NumSamples; ++i)
    {
        float2 e = Hammersley(i, NumSamples, random);
        float cosTheta, sinTheta;
        float3 sampleDirTS = UniformSampleHemisphere(e, cosTheta, sinTheta).xyz;
        float3 sampleDirWS = TangentToWorld(sampleDirTS.xyz, outputNormalWS);
        float3 colorSample = EnvironmentMap.SampleLevel(EnvMapSampler, sampleDirWS, 0).rgb;
        irradiance += colorSample * cosTheta * sinTheta;
    }
    irradiance = (irradiance / NumSamples) * PI * PI;

    uint3 arrayCoords = uint3(threadID.xy, Face);
    IrradianceCubeMapUAV[arrayCoords] = float4(irradiance, 1.0);
}