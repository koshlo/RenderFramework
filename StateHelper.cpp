#include "StateHelper.h"
#include "Renderer.h"

void StateHelper::SetShaderParam(const char* name, float2 float2Const)
{
	_renderer->setShaderConstant2f(name, float2Const);
}

void StateHelper::SetShaderParam(const char* name, float4 float4Const)
{
	_renderer->setShaderConstant4f(name, float4Const);
}

void StateHelper::SetShaderParam(const char* name, float4x4 matrix)
{
	_renderer->setShaderConstant4x4f(name, matrix);
}

void StateHelper::SetShaderTexture(const char* name, TextureID texture)
{
	_renderer->setTexture(name, texture);
}

void StateHelper::SetSamplerState(const char* name, SamplerStateID samplerState)
{
	_renderer->setSamplerState(name, samplerState);
}

void StateHelper::SetUnorderedAccessTexture(const char* name, TextureID texture)
{
	_renderer->setUnorderedAccessTexture(name, texture);
}
