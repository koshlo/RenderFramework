#pragma once

#include "../RenderDefs.h"

class GraphicsDevice;

enum RenderPath
{
	RenderPath_Forward,
	RenderPath_Deferred,

	RenderPath_Count
};

class ShaderCache
{
public:
	ShaderCache(GraphicsDevice* device);

	ShaderID GetGeometryShader(RenderPath renderPath) const;
private:
	GraphicsDevice* _device;
	ShaderID _geometryShaders[RenderPath_Count];
};