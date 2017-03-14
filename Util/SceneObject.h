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
class RenderStateCache;
class RenderQueue;

class SceneObject
{
public:
	bool Load(const std::string& name, const RenderResourceLoader& resourceLoader, ShaderCache& shaderCache, RenderStateCache& stateCache);
	
	void Draw(StateHelper* stateHelper, ShaderID shader, const ShaderData& shaderData) const;
    void Draw(RenderQueue& renderQueue, const RenderState* renderState, uint32 sortKey) const;
	void Draw(RenderQueue& renderQueue, uint32 sortKey) const;
	AABB GetBoundingBox() const;

private:
	Model _model;
	Optional<ObjMaterialLib> _materialLib;
	std::vector<MaterialShaderData*> _batchMaterials;
	RenderState _opaqueForwardState;
	ShaderID _geometryShader;

	void PrepareDrawData(GraphicsDevice& gfxDevice);
};