#ifndef _DRAW_CALL_H_
#define _DRAW_CALL_H_

#include "RenderDefs.h"
#include "Platform.h"

class Model;
class ShaderData;
class StateHelper;
class Renderer;

typedef void(*RenderFunc)(Renderer* renderer, StateHelper* stateHelper, void* data);


struct BatchDrawCall
{
	BatchDrawCall() :
		shader(SHADER_NONE),
		geometry(nullptr),
		shaderData(nullptr),
		shaderDataCount(0),
		batchNumber(0)
	{}

	ShaderID shader;
	Model* geometry;
	ShaderData* shaderData;
	uint shaderDataCount;
	uint batchNumber;

	static void Render(Renderer* renderer, StateHelper* stateHelper, void* data);
};

#endif // _DRAW_CALL_H_