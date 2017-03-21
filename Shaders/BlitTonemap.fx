#include "BlitVertex.inc.fx"

[Vertex shader]

PsIn main(uint vertexID : SV_VertexID)
{
    return QuadVertex(vertexID);
}

[Fragment shader]

float3 BurgessDawson(float3 color)
{
    float3 x = max((float3) (0.0), color - 0.004);
    float3 retColor = (x * (6.2 * x + .5)) / (x * (6.2 * x + 1.7) + 0.06);
    return retColor;
}

float4 main(PsIn psIn) : SV_Target
{
    const float Exposure = 0.1;
    float4 sourceColor = SourceTexture.Sample(SourceSampler, psIn.texCoord);
    float3 tonemapped = BurgessDawson(sourceColor.rgb * Exposure);
    return float4(tonemapped, 1.0);
}