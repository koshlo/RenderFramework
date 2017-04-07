#include "BRDF.inc.fx"
#include "Lighting.data.fx"

struct MaterialInfo
{
    float rougness;
    float3 albedo;
    float3 specular;
};

struct SurfaceInfo
{
    float3 posWS;
    float3 normal;
    float3 viewDir;
    float3 lightDir;
};

float3 ComputeDirectLight(const in MaterialInfo material, const in SurfaceInfo surfaceInfo, float3 intensity)
{
    float3 diffuse = ComputeDiffuseBRDF(material.albedo);
    float3 specular = ComputeSpecularBRDF(material.specular, material.rougness, surfaceInfo.normal, surfaceInfo.viewDir, surfaceInfo.lightDir);
    return intensity * (diffuse * (1.0f - specular) + specular) * ClampDot(surfaceInfo.normal, surfaceInfo.lightDir);
}

float GetSqrDistance(float3 lhs, float3 rhs)
{
    float3 dif = lhs - rhs;
    return dot(dif, dif);
}

float3 ComputeIndirectLight(const in MaterialInfo material, const in SurfaceInfo surfaceInfo)
{
    uint nearestProbe = 0;
    float maxSqrDistance = 0;
    for (uint i = 0; i < ProbeCount; ++i)
    {
        float sqrDist = GetSqrDistance(surfaceInfo.posWS, ProbeLocations[i].xyz);
        if (sqrDist > maxSqrDistance)
        {
            maxSqrDistance = sqrDist;
            nearestProbe = i;
        }
    }

    float3 irradiance = IrradianceProbes.Sample(IrradianceSampler, float4(surfaceInfo.normal, nearestProbe));
    return ComputeDiffuseBRDF(material.albedo * irradiance);
}