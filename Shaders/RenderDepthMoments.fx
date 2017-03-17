[Vertex shader]

#include "View.data.fx"

struct VsIn
{
	float4 Position : Position;
};

float4 main(VsIn In) : SV_Position
{
	return mul(ViewProjection, In.Position);
}

[Fragment shader]

#include "ExpWarping.inc.fx"

float ComputeBias(in float depth)
{
	float dx = ddx_coarse(depth);
	float dy = ddx_coarse(depth);
	return 0.25 * (dx*dx + dy*dy);
}

float2 main(float4 position : SV_Position) : SV_Target
{
	float warpedDepth = WarpDepth(position.z);
	float2 depthAndSqrDepth = float2(warpedDepth, warpedDepth*warpedDepth);
	return depthAndSqrDepth;
}