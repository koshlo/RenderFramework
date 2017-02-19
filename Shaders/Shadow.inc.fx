#define MIN_VARIANCE 0.005f
#define LIGHT_BLEAD_REDUCTION 0.3f

#include "Shadow.data.fx"

#define ExponentialWarpPower ExpPower.x

float WarpDepth(float d)
{
	return exp(d * ExponentialWarpPower);
}

float UnwarpDepth(float warpedDepth)
{
	return log(warpedDepth) / ExponentialWarpPower;
}

float RemapShadow(float shadowValue, float amount)  
{  
	return smoothstep(amount, 1, shadowValue);  
}  

float GetShadowingUpperBound(float2 moments, float sceneDepth)
{
	float variance = moments.y - moments.x*moments.x;
	variance = max(variance, MIN_VARIANCE);
	float depthDiff = WarpDepth(sceneDepth) - moments.x;
	float upperBound = variance / (variance + depthDiff*depthDiff);
	return max(depthDiff > 0, RemapShadow(upperBound, LIGHT_BLEAD_REDUCTION));
}

float GetShadow(float4 position)
{
	float4 lightSpaceCoords = mul(ShadowMapProjection, position);
	lightSpaceCoords.xy = ( lightSpaceCoords.xy + float2(1, -1) ) * float2(0.5, -0.5);

	float2 moments = ShadowMap.Sample(VarianceSampler, lightSpaceCoords.xy);

	return GetShadowingUpperBound(moments, lightSpaceCoords.z);
}
