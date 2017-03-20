#include "Blit.data.fx"

struct PsIn
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

PsIn QuadVertex(uint vertexID)
{
    float2 pos = float2(vertexID / 2, vertexID % 2);
    
    PsIn vsOut;
    vsOut.texCoord = float2(pos.x, (1.0f - pos.y));
    vsOut.position = float4(pos * 2.0f - 1.0f, 0.0f, 1.0f);
    return vsOut;
}