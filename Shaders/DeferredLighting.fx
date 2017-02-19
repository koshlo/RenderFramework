
struct PsIn
{
	float4 Position: SV_Position;
	float2 TexCoord: TexCoord;
};

[Vertex shader]

PsIn main(uint VertexID: SV_VertexID)
{
	PsIn Out;

	// Produce a fullscreen triangle
	float x = (VertexID == 2)?  3.0f : -1.0f;
	float y = (VertexID == 0)? -3.0f :  1.0f;

	Out.Position = float4(x, y, 0.5f, 1.0f);
	Out.TexCoord = float2(0.5f * x, -0.5f * y) + 0.5f;

	return Out;
}


[Fragment shader]

Texture2D BaseRT;
Texture2D NormalRT;
float3 SunDirection;
float3 SunIntensity;
float3 Ambient;

SamplerState PointClamp
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 main(PsIn In) : SV_Target
{
	float4 outColor;
	float3 normal = NormalRT.Sample(PointClamp, In.TexCoord).xyz;
	float sunLighting = saturate( dot(normal, SunDirection) );
	float4 material = BaseRT.Sample(PointClamp, In.TexCoord);
	float3 diffuseReflectance = material.rgb;
	outColor.rgb = diffuseReflectance * (sunLighting * SunIntensity + Ambient);
	outColor.a = 1;
	return outColor;
}