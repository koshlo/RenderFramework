#include "RenderState.h"
#include "Renderer.h"

template <typename T>
uint64 UnionCast(const T& val)
{
	static_assert(sizeof(T) < sizeof(uint64));

	union Uint64Union
	{
		uint64 intVal;
		T structVal;
	};

	Uint64Union unionVal;
	unionVal.structVal = val;
	return unionVal.intVal;
}

uint64 DescToKey(const RenderStateDesc& desc)
{
	return UnionCast(desc);
}

const RenderState& RenderStateCache::GetRenderState(GraphicsDevice& gfxDevice, const RenderStateDesc& stateDesc)
{
	uint64 key = DescToKey(stateDesc);
	std::pair<RenderStateMap::iterator, bool> keyInserted = _renderStates.emplace(key, RenderState());
	if (!keyInserted.second)
	{
		RenderState& renderState = keyInserted.first->second;
		renderState.depthState = gfxDevice.addDepthState(stateDesc.depthTest, stateDesc.depthWrite, stateDesc.depthOp);
		renderState.blendState = gfxDevice.addBlendState(ONE, ZERO);
		renderState.rasterizerState = gfxDevice.addRasterizerState(stateDesc.cullState, SOLID, false);		
	}
}
