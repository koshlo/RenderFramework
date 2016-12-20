#ifndef _DRAW_CALL_H_
#define _DRAW_CALL_H_

#include "RenderDefs.h"
#include "Platform.h"

class Model;
class ShaderData;
class StateHelper;
class GraphicsDevice;

typedef void(*RenderFunc)(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data);


struct BatchDrawCall
{
	BatchDrawCall() :
		geometry(nullptr),
		shaderData(nullptr),
		shaderDataCount(0),
		batchNumber(0)
	{}

	Model* geometry;
	ShaderData* shaderData;
	uint shaderDataCount;
	uint batchNumber;

	static void Render(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data);
};

#endif // _DRAW_CALL_H_