#include "RenderQueue.h"
#include "StateHelper.h"
#include "Renderer.h"
#include "ShaderData.h"

#include <algorithm>

static const size_t OffsetNextCommand = 0;
static const size_t OffsetRenderFunc = OffsetNextCommand + sizeof(RenderCommand);
static const size_t OffsetStatePtr = OffsetRenderFunc + sizeof(RenderFunc);
static const size_t OffsetData = OffsetStatePtr + sizeof(RenderStatePtr);

template <typename T, size_t Offset>
T* GetCommandData(RenderCommand command)
{
	return reinterpret_cast<T*>(static_cast<uint8*>(command) + Offset);
}

RenderCommand* GetNextCommand(RenderCommand command)
{
	return GetCommandData<RenderCommand, OffsetNextCommand>(command);
}

void StoreNextCommand(RenderCommand command, RenderCommand nextCommand)
{
	*( GetNextCommand(command) ) = nextCommand;
}

RenderFunc* GetRenderFunc(RenderCommand command)
{
	return GetCommandData<RenderFunc, OffsetRenderFunc>(command);
}

void StoreRenderFunc(RenderCommand command, RenderFunc func)
{
	*(GetRenderFunc(command)) = func;
}

RenderStatePtr* GetRenderStatePtr(RenderCommand command)
{
	return GetCommandData<RenderStatePtr, OffsetStatePtr>(command);
}

void StoreRenderStatePtr(RenderCommand command, RenderStatePtr renderState)
{
	*(GetRenderStatePtr(command)) = renderState;
}

void* GetData(RenderCommand command)
{
	return static_cast<uint8*>(command) + OffsetData;
}

RenderQueue::RenderQueue() : 
	_currentCommand(0),
	_clearRT(false),
	_clearDepth(false),
	_clearColor(0),
	_depthClearVal(0)
{
	_buffer.resize(BufferSize);
	_commands[_currentCommand] = _buffer.data();
	StoreNextCommand(_commands[_currentCommand], _commands[_currentCommand]);
}

void RenderQueue::SetRenderTargets(TextureID* rts, uint count, TextureID depthRT)
{
	ASSERT(count <= MaxRenderTargets);
	std::copy(rts, rts + count, _renderTargets);
	_numRenderTargets = count;
	_depthRT = depthRT;
}

void RenderQueue::SetClear(bool clearRT, bool clearDepth, float4 clearColor, float depthClearVal)
{
	_clearRT = clearRT;
	_clearDepth = clearDepth;
	_clearColor = clearColor;
	_depthClearVal = depthClearVal;
}

void RenderQueue::SetShaderData(ShaderData** shaderData, uint count)
{
	ASSERT(count <= MaxShaderDataPerQueue);
	std::copy(shaderData, shaderData + count, _shaderData);
	_numShaderData = count;
}

void RenderQueue::Sort()
{
	std::sort(_sortingKeys, _sortingKeys + _currentCommand, [this](const SortKeyType& left, const SortKeyType& right)
	{
		bool less = left < right;
		if (less)
		{
			std::swap(_commands[left], _commands[right]);
		}
		return less;
	});
}

void RenderQueue::SubmitAll(GraphicsDevice* gfxDevice, StateHelper* stateHelper)
{
	gfxDevice->reset();
	gfxDevice->changeRenderTargets(_renderTargets, _numRenderTargets, _depthRT);
	gfxDevice->clear(_clearRT, _clearDepth, _clearColor, _depthClearVal);
	for (uint i = 1; i <= _currentCommand; ++i)
	{
		RenderCommand command = _commands[i];
		RenderFunc func = *GetRenderFunc(command);
		RenderStatePtr renderState = *GetRenderStatePtr(command);
		stateHelper->Apply(renderState);
		for (uint iShaderData = 0; iShaderData < _numShaderData; ++iShaderData)
		{
			_shaderData[iShaderData]->Apply(stateHelper);
		}
		func(gfxDevice, stateHelper, GetData(command));
	}
	_currentCommand = 0;
}

TextureID RenderQueue::GetDepthTarget() const
{
	return _depthRT;
}

void RenderQueue::DispatchCompute(StateHelper* stateHelper, const DispatchGroup& group, ShaderID shader, ShaderData** shaderData, uint numShaderData)
{
	GraphicsDevice* device = stateHelper->GetDevice();
	device->reset(RESET_UAV);
	device->setShader(shader);
	for (uint i = 0; i < numShaderData; ++i)
	{
		shaderData[i]->Apply(stateHelper);
	}
	device->dispatchCompute(group.x, group.y, group.z);
}

TextureID RenderQueue::GetRenderTarget(uint index) const
{
	return _renderTargets[index];
}

void* RenderQueue::AddDrawCommand(uint dataSize, RenderFunc renderFunc, const RenderState* renderState)
{
	RenderCommand* nextCommandPtr = GetNextCommand(_commands[_currentCommand]);
	RenderCommand nextCommand = *nextCommandPtr;
	StoreRenderFunc(nextCommand, renderFunc);
	StoreRenderStatePtr(nextCommand, renderState);
	StoreNextCommand(nextCommand, static_cast<uint8*>(nextCommand) + OffsetData + dataSize);
	_commands[++_currentCommand] = nextCommand;
	return GetData(nextCommand);
}
