#include "RenderState.h"
#include "Renderer.h"

template <typename IntT, typename T>
IntT UnionCast(const T& val)
{
	static_assert(sizeof(T) <= sizeof(uint64), "struct should be no greater than the size of uint64");

	union CastUnion
	{
        IntT intVal;
		T structVal;
	};

    CastUnion unionVal;
	unionVal.structVal = val;
	return unionVal.intVal;
}

uint64 DescToKey(const RenderStateDesc& desc)
{
	return UnionCast<uint64>(desc);
}

RenderState RenderStateCache::GetRenderState(const RenderStateDesc& stateDesc)
{
	uint64 key = DescToKey(stateDesc);
	std::pair<RenderStateMap::iterator, bool> keyInserted = _renderStates.emplace(key, RenderState());
	if (keyInserted.second)
	{
		RenderState& renderState = keyInserted.first->second;
        renderState.shader = stateDesc.shader;
		renderState.depthState = _device->addDepthState(stateDesc.depthTest != 0, stateDesc.depthWrite != 0, stateDesc.depthOp);
		renderState.blendState = _device->addBlendState(ONE, ZERO);
		renderState.rasterizerState = _device->addRasterizerState(stateDesc.cullState, SOLID, false);
	}
	return std::ref(keyInserted.first->second);
}

SamplerStateID RenderStateCache::GetSamplerState(const SamplerStateDesc& samplerDesc)
{
    uint32 key = UnionCast<uint32>(samplerDesc);
    std::pair<SamplerStateMap::iterator, bool> keyInserted = _samplerStates.emplace(key, SS_NONE);
    if (keyInserted.second)
    {
        keyInserted.first->second = _device->addSamplerState((Filter)samplerDesc.filterMode,
            (AddressMode)samplerDesc.addressModeS,
            (AddressMode)samplerDesc.addressModeT,
            (AddressMode)samplerDesc.addressModeR);
    }
    return keyInserted.first->second;
}
