#include "BRDF.inc.fx"
#include "Lighting.data.fx"

#define BIG_FLOAT 1000000.0f

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
    uint2 nearestProbe = 0;
    float2 minSqrDistance = BIG_FLOAT;
    for (uint i = 0; i < ProbeCount; ++i)
    {
        float sqrDist = GetSqrDistance(surfaceInfo.posWS, ProbeLocations[i].xyz);
        bool inFront = dot(surfaceInfo.posWS, ProbeLocations[i].xyz) > 0;
        if (sqrDist < minSqrDistance.x && inFront)
        {
            minSqrDistance.y = minSqrDistance.x;
            nearestProbe.y = nearestProbe.x;
            minSqrDistance.x = sqrDist;
            nearestProbe.x = i;
        }
        else if (sqrDist < minSqrDistance.y && inFront)
        {
            minSqrDistance.y = sqrDist;
            nearestProbe.y = i;
        }
    }

    float3 irradiance1 = IrradianceProbes.Sample(IrradianceSampler, float4(surfaceInfo.normal, nearestProbe.x));
    float3 irradiance2 = IrradianceProbes.Sample(IrradianceSampler, float4(surfaceInfo.normal, nearestProbe.y));
    float fade = minSqrDistance.x / (minSqrDistance.x + minSqrDistance.y);

    return ComputeDiffuseBRDF(material.albedo * lerp(irradiance1, irradiance2, fade));
}