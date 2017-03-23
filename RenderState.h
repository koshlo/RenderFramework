#pragma once

#include "RenderDefs.h"
#include "Platform.h"
#include <unordered_map>

class GraphicsDevice;

struct RenderState
{
	ShaderID shader;
	DepthStateID depthState;
	BlendStateID blendState;
	RasterizerStateID rasterizerState;
};

struct RenderStateDesc
{
	ShaderID shader;
	int32 depthTest : 8;
	int32 depthWrite : 8;
	int32 depthOp : 8;
	int32 cullState : 8;
};

struct SamplerStateDesc
{
    int32 filterMode : 8;
    int32 addressModeS : 8;
    int32 addressModeT : 8;
    int32 addressModeR : 8;
};

class RenderStateCache
{
public:
	RenderStateCache(GraphicsDevice* gfxDevice) : _device(gfxDevice) {}
	RenderState GetRenderState(const RenderStateDesc& stateDesc);
    SamplerStateID GetSamplerState(const SamplerStateDesc& samplerDesc);
private:
	typedef std::unordered_map<uint64, RenderState> RenderStateMap;
    typedef std::unordered_map<uint32, SamplerStateID> SamplerStateMap;

	RenderStateMap _renderStates;
    SamplerStateMap _samplerStates;
	GraphicsDevice* _device;
};
