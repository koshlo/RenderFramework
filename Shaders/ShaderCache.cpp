#include "ShaderCache.h"

#include "../Util/Helpers.h"
#include "../Platform.h"
#include "../Renderer.h"

namespace
{
	const char* GeometryShaders[] =
	{
		"ForwardGeometry.fx",
		"DeferredGeometry.fx"
	};
}

ShaderCache::ShaderCache(GraphicsDevice* device)
	: _device(device)
{
	static_assert(RenderPath_Count == array_size(GeometryShaders), "Invalid mapping of shaders");

	for (uint i = 0; i < array_size(_geometryShaders); ++i)
	{
		_geometryShaders[i] = _device->addShader(GeometryShaders[i]);
	}
}

ShaderID ShaderCache::GetGeometryShader(RenderPath renderPath) const
{
	return _geometryShaders[renderPath];
}
