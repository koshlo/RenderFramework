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

class RenderStateCache
{
public:
	RenderStateCache(GraphicsDevice* gfxDevice) : _device(gfxDevice) {}
	const RenderState& GetRenderState(const RenderStateDesc& stateDesc);
private:
	typedef std::unordered_map<uint64, RenderState> RenderStateMap;

	RenderStateMap _renderStates;
	GraphicsDevice* _device;
};
