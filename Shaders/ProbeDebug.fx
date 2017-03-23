#include "ProbeDebug.data.fx"

struct VsIn
{
    float4 position : Position;
    float3 normal : Normal;
};

struct PsIn
{
    float4 position : SV_Position;
    float3 normal : Normal;
};

[Vertex shader]

#include "View.data.fx"

PsIn main(VsIn vertex)
{
    PsIn psOut;
    float3 scaledPos = vertex.position.xyz + TranslationScale.w * vertex.normal;
    float4 translatedPos = float4(scaledPos + TranslationScale.xyz, vertex.position.w);
    psOut.position = mul(ViewProjection, translatedPos);
    psOut.normal = vertex.normal;
    return psOut;
}

[Fragment shader]

float4 main(PsIn psIn) : SV_Target
{
    return CubeMap.Sample(CubeMapSampler, psIn.normal);
}