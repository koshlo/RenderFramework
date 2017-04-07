
struct VsIn
{
	float4 Position : Position;
	float3 Normal : Normal;
    float3 Tangent : Tangent;
    float3 Bitangent : Bitangent;
    float2 TexCoord : TexCoord;
};

struct PsIn
{
	float4 Position : SV_Position;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float3 Bitangent : Bitangent;
    float3 PositionWS : PositionWS;
    float2 TexCoord : TexCoord;
};

#include "View.data.fx"

[Vertex shader]

PsIn main(VsIn vsIn)
{
	PsIn psOut;

	psOut.Position = mul(ViewProjection, vsIn.Position);
    psOut.Tangent = vsIn.Tangent;
    psOut.Bitangent = vsIn.Bitangent;
    psOut.Normal = vsIn.Normal;
    psOut.TexCoord = vsIn.TexCoord;
    psOut.PositionWS = vsIn.Position.xyz;
	return psOut;
}


[Fragment shader]

#include "Shadow.inc.fx"
#include "Lighting.inc.fx"
#include "Material.data.fx"

float4 ScreenToWorld(float2 screenCoords, float depth)
{
	float2 normalizedScreenCoords = (float2(2, -2) * screenCoords) / Viewport.xy + float2(-1, 1);
	float4 clipCoords = float4(normalizedScreenCoords, depth, 1);
	float4 worldCoords = mul(InvViewProjection, clipCoords);
	return worldCoords / worldCoords.w;
}

MaterialInfo GetMaterialInfo(float2 uv)
{
    MaterialInfo outMat;
    outMat.rougness = RoughnessMap.Sample(MaterialSampler, uv);
    
    float metallic = MetallicMap.Sample(MaterialSampler, uv);
    float3 albedo = AlbedoMap.Sample(MaterialSampler, uv);
    outMat.albedo = albedo * (1.0f - metallic);
    outMat.specular = lerp(0.03f, albedo, metallic);
    return outMat;
}

SurfaceInfo GetSurfaceInfo(float2 uv, float3x3 tangentToWS, float3 posWS)
{
    SurfaceInfo outSurfInfo;
    float3 normalTS = NormalMap.Sample(MaterialSampler, uv) * 2.0f - 1.0f;
    outSurfInfo.posWS = posWS;
    outSurfInfo.normal = normalize(mul(normalTS, tangentToWS));
    outSurfInfo.lightDir = normalize(SunDirection);
    outSurfInfo.viewDir = normalize(EyePos.xyz - posWS);
    return outSurfInfo;
}

float4 main(PsIn psIn) : SV_Target
{
	float4 outColor;
	float visibility = GetShadow(float4(psIn.PositionWS, 1.0f));
    float2 uv = float2(psIn.TexCoord.x, 1.0f - psIn.TexCoord.y);
    MaterialInfo matInfo = GetMaterialInfo(uv);
    SurfaceInfo surfInfo = GetSurfaceInfo(uv, float3x3(psIn.Tangent, psIn.Bitangent, psIn.Normal), psIn.PositionWS);
    outColor.rgb = ComputeDirectLight(matInfo, surfInfo, SunIntensity) * visibility;
    outColor.rgb += ComputeIndirectLight(matInfo, surfInfo);
	outColor.a = 1;
	return outColor;
}
