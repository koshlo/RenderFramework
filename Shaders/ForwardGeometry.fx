
struct VsIn
{
	float4 Position : Position;
	float3 Normal   : Normal;
};

struct PsIn
{
	float4 Position : SV_Position;
	float3 Normal   : Normal;
};

[Vertex shader]

float4x4 ViewProj;
float3 CamPos;

PsIn main(VsIn In)
{
	PsIn Out;

	Out.Position = mul(ViewProj, In.Position);
	Out.Normal   = In.Normal;

	return Out;
}


[Fragment shader]

#include "Shadow.inc.fx"
#include "../RenderFramework/Shaders/Material.data.fx"

float3 MaterialDiffuse;
float3 SunDirection;
float3 SunIntensity;
float3 Ambient;
float2 Viewport;
float4x4 InvViewProjection;

float4 ScreenToWorld(float2 screenCoords, float depth)
{
	float2 normalizedScreenCoords = (float2(2, -2) * screenCoords) / Viewport.xy + float2(-1, 1);
	float4 clipCoords = float4(normalizedScreenCoords, depth, 1);
	float4 worldCoords = mul(InvViewProjection, clipCoords);
	return worldCoords / worldCoords.w;
}

float4 main(PsIn In) : SV_Target
{
	float4 outColor;
	float sunLighting = saturate( dot(In.Normal, SunDirection) );
	float visibility = GetShadow(ScreenToWorld(In.Position.xy, In.Position.z));
	outColor.rgb = MaterialDiffuse * (sunLighting * SunIntensity * visibility + Ambient);
	outColor.a = 1;
	return outColor;
}
