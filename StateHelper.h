#ifndef _STATE_HELPER_H_
#define _STATE_HELPER_H_

#include "RenderDefs.h"

class Renderer;

class StateHelper
{
public:
	StateHelper(Renderer* renderer) : _renderer(renderer) {};

	void SetShaderParam(const char* name, float2 float2Const);
	void SetShaderParam(const char* name, float4 float4Const);
	void SetShaderParam(const char* name, float4x4 matrix);
	void SetShaderTexture(const char* name, TextureID texture);
	void SetSamplerState(const char* name, SamplerStateID samplerState);
	void SetUnorderedAccessTexture(const char* name, TextureID texture);
private:
	Renderer* _renderer;
};

#endif // _STATE_HELPER_H_