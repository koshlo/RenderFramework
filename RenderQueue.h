#ifndef _RENDERER_QUEUE_H_
#define _RENDERER_QUEUE_H_

#include "DrawCall.h"
#include <vector>

typedef void* RenderCommand;

class RenderQueue
{
public:
	RenderQueue();

	template<typename T>
	T& AddRenderCommand()
	{
		RenderFunc renderFunc = T::Render;
		void* data = AddDrawCommand(sizeof(T), renderFunc);
		T* commandData = static_cast<T*>(data);
		return *commandData;
	}
	
	void SubmitAll(Renderer* renderer, StateHelper* stateHelper);
private:
	uint GetAlignedOffset(uint size, uint alignment)
	{
		ASSERT(alignment != 0);

		uint alignMask = ~(alignment - 1);
		return (size + alignment - 1) & alignMask;
	}

	void* AddDrawCommand(uint dataSize, RenderFunc renderFunc);

	static const uint BufferSize = 1024 * 16;
	std::vector<uint8> _buffer;

	static const uint MaxCommands = 512;
	RenderCommand _commands[MaxCommands];
	uint _sortingKeys[MaxCommands];
	uint _currentCommand;
};

#endif // _RENDERER_QUEUE_H_