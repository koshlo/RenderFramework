#include "StateHelper.h"
#include "Renderer.h"
#include "RenderState.h"

void StateHelper::SetShaderParam(const char* name, uint uintConst)
{
	_device->setShaderConstant1i(name, uintConst);
}

void StateHelper::SetShaderParam(const char* name, float floatConst)
{
	_device->setShaderConstant1f(name, floatConst);
}

void StateHelper::SetShaderParam(const char* name, float2 float2Const)
{
	_device->setShaderConstant2f(name, float2Const);
}

void StateHelper::SetShaderParam(const char* name, float3 float3Const)
{
	_device->setShaderConstant3f(name, float3Const);
}

void StateHelper::SetShaderParam(const char* name, float4 float4Const)
{
	_device->setShaderConstant4f(name, float4Const);
}

void StateHelper::SetShaderParam(const char* name, float4x4 matrix)
{
	_device->setShaderConstant4x4f(name, matrix);
}

void StateHelper::SetShaderArray(const char* name, const float4* values, uint count)
{
	_device->setShaderConstantArray4f(name, values, count);
}

void StateHelper::SetShaderTexture(const char* name, TextureID texture)
{
	_device->setTexture(name, texture);
}

void StateHelper::SetSamplerState(const char* name, SamplerStateID samplerState)
{
	_device->setSamplerState(name, samplerState);
}

void StateHelper::SetUnorderedAccessTexture(const char* name, TextureID texture)
{
	_device->setUnorderedAccessTexture(name, texture);
}

void StateHelper::ApplyConstants()
{
	_device->applyConstants();
}

void StateHelper::ApplyTextures()
{
	_device->applyTextures();
}

void StateHelper::Apply(const RenderState* renderState)
{
	_device->setShader(renderState->shader);
	_device->setDepthState(renderState->depthState);
	_device->setBlendState(renderState->blendState);
	_device->setRasterizerState(renderState->rasterizerState);
	_device->apply();
}
