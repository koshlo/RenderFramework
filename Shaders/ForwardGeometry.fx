
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

#include "View.data.fx"

[Vertex shader]

PsIn main(VsIn In)
{
	PsIn Out;

	Out.Position = mul(ViewProjection, In.Position);
	Out.Normal   = In.Normal;

	return Out;
}


[Fragment shader]

#include "Material.data.fx"
#include "Lighting.data.fx"
#include "Shadow.inc.fx"

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
	float visibility = 1.0f;//GetShadow(ScreenToWorld(In.Position.xy, In.Position.z));
    outColor.rgb = float3(0.8f, 1.0f, 0.8f) * (sunLighting * SunIntensity * visibility + Ambient);
	outColor.a = 1;
	return outColor;
}
