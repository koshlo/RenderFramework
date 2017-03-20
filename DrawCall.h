#ifndef _DRAW_CALL_H_
#define _DRAW_CALL_H_

#include "RenderDefs.h"
#include "Platform.h"

class Model;
class ShaderData;
class StateHelper;
class GraphicsDevice;
class RenderStateCache;

typedef void(*RenderFunc)(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data);

struct IndexedInstancedDrawCall
{
	VertexBufferID vertexBuffer;
	IndexBufferID indexBuffer;
	uint indexCount;
	uint instanceCount;
	uint firstIndex;
	uint firstVertex;
	uint firstInstance;

	static void Render(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data);
};

struct BatchDrawCall
{
	BatchDrawCall() :
		geometry(nullptr),
		batchNumber(0)
	{}

	const Model* geometry;
	uint batchNumber;

	static void Render(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data);
};
#endif // _DRAW_CALL_H_