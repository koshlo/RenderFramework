#pragma once

#include "Model.h"
#include "ObjMaterialLib.h"
#include "AABB.h"
#include "../StateHelper.h"
#include "../RenderState.h"

class RenderResourceLoader;
class ShaderData;
class SceneShaderData;
class ShaderCache;
class RenderQueue;

class SceneObject
{
public:
	bool Load(const std::string& name, const RenderResourceLoader& resourceLoader, const ShaderCache& shaderCache);
	
	void Draw(StateHelper* stateHelper) const;
	void Draw(StateHelper* stateHelper, ShaderID shader, const ShaderData& shaderData) const;
	void Draw(RenderQueue& renderQueue);
	AABB GetBoundingBox() const;

	static void SetViewProjection(const mat4& viewProj);
	static void SetSunLighting(const vec3& lightDir, const vec3& lightIntensity, const vec3& ambient);
	static void SetViewport(const vec2& viewport);
	static bool SetupGeometryShader(const char* shaderName, GraphicsDevice* gfxDevice);
private:
	Model _model;
	Optional<ObjMaterialLib> _materialLib;
	std::vector<MaterialShaderData*> _batchMaterials;
	RenderState _renderState;

	void PrepareDrawData(GraphicsDevice& gfxDevice);
};