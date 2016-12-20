#pragma once

#include "RenderDefs.h"
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
	bool depthTest : 4;
	bool depthWrite : 4;
	int16 depthOp;
	int16 cullState;
};

class RenderStateCache
{
public:
	const RenderState& GetRenderState(GraphicsDevice& gfxDevice, const RenderStateDesc& stateDesc);
private:
	typedef std::unordered_map<uint64, RenderState> RenderStateMap;

	RenderStateMap _renderStates;
};
