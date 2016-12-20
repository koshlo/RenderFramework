#include "SceneObject.h"
#include "ResourceLoader.h"
#include "../Shaders/Material.data.fx"
#include "../Shaders/Scene.data.fx"
#include "../Shaders/ShaderCache.h"
#include "../RenderQueue.h"
#include "../DrawCall.h"

bool SceneObject::Load(const std::string& name, const RenderResourceLoader& resourceLoader, const ShaderCache& shaderCache, RenderStateCache& stateCache)
{
	if ( _model.loadObj(name.c_str()) )
	{
		const MaterialLibResId& matName = _model.getMaterialLib();
		if (matName.isValid())
		{
			resourceLoader.SyncLoad(matName, &_materialLib);
		}
	}
	if (_materialLib.isValid())
	{
		_geometryShader = shaderCache.GetGeometryShader(RenderPath_Forward);
		ASSERT(_geometryShader != SHADER_NONE);
		PrepareDrawData(resourceLoader.GetGraphicsDevice());

		RenderStateDesc stateDesc;
		stateDesc.shader = _geometryShader;
		stateDesc.depthWrite = true;
		stateDesc.depthTest = true;
		stateDesc.depthOp = GEQUAL;
		stateDesc.cullState = CULL_BACK;

		_opaqueRenderState = stateCache.GetRenderState(stateDesc);
	}
	return _materialLib.isValid();
}

void SceneObject::PrepareDrawData(GraphicsDevice& gfxDevice)
{
	_batchMaterials.resize(_model.getBatchCount());
	for (uint i = 0; i < _model.getBatchCount(); ++i)
	{
		const std::string& matName = _model.getBatchMaterialName(i);
		_batchMaterials[i] = _materialLib.getVal().getMaterialShaderData(matName);
	}
	_model.makeDrawable(&gfxDevice, true, _geometryShader);
}

void SceneObject::Draw(StateHelper* stateHelper, ShaderID shader, const ShaderData& shaderData) const
{
	stateHelper->GetDevice()->setShader(shader);
	shaderData.Apply(stateHelper);
	for (uint i = 0; i < _model.getBatchCount(); ++i)
	{
		_model.drawBatch(stateHelper->GetDevice(), i);
	}
}

void SceneObject::Draw(RenderQueue& renderQueue, uint32 sortKey)
{
	for (uint32 i = 0; i < _model.getBatchCount(); ++i)
	{
		BatchDrawCall& drawCall = renderQueue.AddRenderCommand<BatchDrawCall>(sortKey + i, &_opaqueRenderState);
		drawCall.batchNumber = i;
		drawCall.geometry = &_model;
		drawCall.shaderData = _batchMaterials[i];
		drawCall.shaderDataCount = 1;
	}
}

AABB SceneObject::GetBoundingBox() const
{
	float aabbCoordsMin[3];
	float aabbCoordsMax[3];
	_model.getBoundingBox(_model.findStream(TYPE_VERTEX), aabbCoordsMin, aabbCoordsMax);
	return AABB(aabbCoordsMin, aabbCoordsMax);
}
