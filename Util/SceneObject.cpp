#include "SceneObject.h"
#include "ResourceLoader.h"
#include "../Shaders/Material.data.fx"
#include "../Shaders/Scene.data.fx"
#include "../Shaders/ShaderCache.h"
#include "../RenderQueue.h"
#include "../DrawCall.h"

SceneShaderData SceneObject::_sceneShaderData;
ShaderID SceneObject::_geometryShader = SHADER_NONE;

bool SceneObject::Load(const std::string& name, const RenderResourceLoader& resourceLoader, const ShaderCache& shaderCache)
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
	}
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

void SceneObject::Draw(StateHelper* stateHelper) const
{
	GraphicsDevice* gfxDevice = stateHelper->GetDevice();
	gfxDevice->setShader(_geometryShader);
	_sceneShaderData.Apply(stateHelper);
	for (uint i = 0; i < _model.getBatchCount(); ++i)
	{
		_batchMaterials[i]->Apply(stateHelper);
		_model.drawBatch(gfxDevice, i);
	}
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

void SceneObject::Draw(RenderQueue& renderQueue)
{
	renderQueue.AddRenderCommand<BatchDrawCall>()
}

AABB SceneObject::GetBoundingBox() const
{
	float aabbCoordsMin[3];
	float aabbCoordsMax[3];
	_model.getBoundingBox(_model.findStream(TYPE_VERTEX), aabbCoordsMin, aabbCoordsMax);
	return AABB(aabbCoordsMin, aabbCoordsMax);
}

void SceneObject::SetViewProjection(const mat4& viewProj)
{
	_sceneShaderData.SetViewProjection(viewProj);
	_sceneShaderData.SetInvViewProjection(!viewProj);
}

void SceneObject::SetSunLighting(const vec3& lightDir, const vec3& lightIntensity, const vec3& ambient)
{
	_sceneShaderData.SetSunDirection(lightDir);
	_sceneShaderData.SetSunIntensity(lightIntensity);
	_sceneShaderData.SetAmbient(ambient);
}

void SceneObject::SetViewport(const vec2& viewport)
{
	_sceneShaderData.SetViewport(viewport);
}

bool SceneObject::SetupGeometryShader(const char* shaderName, GraphicsDevice* gfxDevice)
{
	_geometryShader = gfxDevice->addShader(shaderName);
	return _geometryShader != SHADER_NONE;
}
