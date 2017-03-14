#include "ExpWarping.data.fx"

#define ExponentialWarpPower ExpPower.x

float WarpDepth(float d)
{
	return exp(d * ExponentialWarpPower);
}

float UnwarpDepth(float warpedDepth)
{
	return log(warpedDepth) / ExponentialWarpPower;
}
