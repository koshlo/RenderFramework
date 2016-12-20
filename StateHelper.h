#ifndef _STATE_HELPER_H_
#define _STATE_HELPER_H_

#include "RenderDefs.h"

class GraphicsDevice;
struct RenderState;

typedef unsigned int uint;

class StateHelper
{
public:
	StateHelper(GraphicsDevice* gfxDevice) : _device(gfxDevice) {};

	void SetShaderParam(const char* name, uint uintConst);
	void SetShaderParam(const char* name, float floatConst);
	void SetShaderParam(const char* name, float2 float2Const);
	void SetShaderParam(const char* name, float3 float3Const);
	void SetShaderParam(const char* name, float4 float4Const);
	void SetShaderParam(const char* name, float4x4 matrix);

	void SetShaderArray(const char* name, const float4* values, uint count);
	
	void SetShaderTexture(const char* name, TextureID texture);
	void SetSamplerState(const char* name, SamplerStateID samplerState);
	void SetUnorderedAccessTexture(const char* name, TextureID texture);

	void ApplyConstants();
	void ApplyTextures();
	void Apply(const RenderState* renderState);

	GraphicsDevice* GetDevice() { return _device; }
private:
	GraphicsDevice* _device;
};

#endif // _STATE_HELPER_H_