#include "BRDF.inc.fx"

struct MaterialInfo
{
    float rougness;
    float3 albedo;
    float3 specular;
};

struct SurfaceInfo
{
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