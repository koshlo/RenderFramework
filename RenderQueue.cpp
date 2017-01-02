#include "RenderQueue.h"
#include "StateHelper.h"
#include "Renderer.h"
#include "ShaderData.h"

#include <algorithm>

static const size_t OffsetNextCommand = 0;
static const size_t OffsetRenderFunc = OffsetNextCommand + sizeof(RenderCommand);
static const size_t OffsetStatePtr = OffsetRenderFunc + sizeof(RenderFunc);
static const size_t OffsetData = OffsetStatePtr + sizeof(DrawCallState);

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

DrawCallState* GetDrawStatePtr(RenderCommand command)
{
	return GetCommandData<DrawCallState, OffsetStatePtr>(command);
}

void StoreDrawStatePtr(RenderCommand command, const DrawCallState& drawState)
{
	*(GetDrawStatePtr(command)) = drawState;
}

void* GetData(RenderCommand command)
{
	return static_cast<uint8*>(command) + OffsetData;
}

RenderQueue::RenderQueue(GraphicsDevice* gfxDevice, uint rtWidth, uint rtHeight, uint rtCount, FORMAT rtFormat, FORMAT depthFormat) :
	_currentCommand(0),
	_clearRT(false),
	_clearDepth(false),
	_clearColor(0),
	_depthClearVal(0)
{
	_buffer.resize(BufferSize);
	_commands[_currentCommand] = _buffer.data();
	StoreNextCommand(_commands[_currentCommand], _commands[_currentCommand]);

	ASSERT(rtCount < MaxRenderTargets);
	for (uint i = 0; i < rtCount; ++i)
	{
		_renderTargets[i] = gfxDevice->addRenderTarget(rtWidth, rtHeight, rtFormat);
	}
	if (depthFormat != FORMAT_NONE)
	{
		_depthRT = gfxDevice->addRenderDepth(rtWidth, rtHeight, 1, depthFormat);
	}
}

void RenderQueue::SetClear(bool clearRT, bool clearDepth, float4 clearColor, float depthClearVal)
{
	_clearRT = clearRT;
	_clearDepth = clearDepth;
	_clearColor = clearColor;
	_depthClearVal = depthClearVal;
}

void RenderQueue::AddShaderData(ShaderData* shaderData)
{
	_shaderData[_numShaderData++] = shaderData;
	ASSERT(_numShaderData < MaxShaderDataPerQueue);
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
		DrawCallState& drawState = *GetDrawStatePtr(command);
		stateHelper->Apply(drawState.renderState);
		drawState.shaderData->Apply(stateHelper);
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

void RenderQueue::DispatchCompute(StateHelper* stateHelper, const DispatchGroup& group, ShaderID shader, const ShaderData** shaderData, uint numShaderData)
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

void* RenderQueue::AddDrawCommand(uint dataSize, RenderFunc renderFunc, const DrawCallState& drawState)
{
	RenderCommand* nextCommandPtr = GetNextCommand(_commands[_currentCommand]);
	RenderCommand nextCommand = *nextCommandPtr;
	StoreRenderFunc(nextCommand, renderFunc);
	StoreDrawStatePtr(nextCommand, drawState);
	StoreNextCommand(nextCommand, static_cast<uint8*>(nextCommand) + OffsetData + dataSize);
	_commands[++_currentCommand] = nextCommand;
	return GetData(nextCommand);
}
