#include "RenderQueue.h"

static const size_t OffsetNextCommand = 0;
static const size_t OffsetRenderFunc = OffsetNextCommand + sizeof(RenderCommand);
static const size_t OffsetData = OffsetRenderFunc + sizeof(RenderFunc);

RenderCommand* GetNextCommand(RenderCommand command)
{
	return reinterpret_cast<RenderCommand*>(static_cast<uint8*>(command) + OffsetNextCommand);
}

void StoreNextCommand(RenderCommand command, RenderCommand nextCommand)
{
	*( GetNextCommand(command) ) = nextCommand;
}

RenderFunc* GetRenderFunc(RenderCommand command)
{
	return reinterpret_cast<RenderFunc*>(static_cast<uint8*>(command) + OffsetRenderFunc);
}

void StoreRenderFunc(RenderCommand command, RenderFunc func)
{
	*(GetRenderFunc(command)) = func;
}

void* GetData(RenderCommand command)
{
	return static_cast<uint8*>(command) + OffsetData;
}

RenderQueue::RenderQueue() : _currentCommand(0)
{
	_buffer.resize(BufferSize);
	_commands[_currentCommand] = _buffer.data();
	StoreNextCommand(_commands[_currentCommand], _commands[_currentCommand]);
}

void RenderQueue::SubmitAll(Renderer* renderer, StateHelper* stateHelper)
{
	for (uint i = 1; i <= _currentCommand; ++i)
	{
		RenderCommand command = _commands[i];
		RenderFunc func = *GetRenderFunc(command);
		func(renderer, stateHelper, GetData(command));
	}
}

void* RenderQueue::AddDrawCommand(uint dataSize, RenderFunc renderFunc)
{
	RenderCommand* nextCommandPtr = GetNextCommand(_commands[_currentCommand]);
	RenderCommand nextCommand = *nextCommandPtr;
	StoreRenderFunc(nextCommand, renderFunc);
	StoreNextCommand(nextCommand, static_cast<uint8*>(nextCommand) + OffsetData + dataSize);
	_commands[++_currentCommand] = nextCommand;
	return GetData(nextCommand);
}
