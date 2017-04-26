#include "TextureAtlas.h"
#include "ArrayMap.h"
#include "Model.h"
#include "../Renderer.h"
#include "../Shaders/Material.data.fx"

void BuildAtlasInfo(const GraphicsDevice& gfxDevice, const Model& model, const ObjMaterialLib& materials, AtlasInfo* atlasInfo)
{
    for (uint i = 0; i < model.getBatchCount(); ++i)
    {
        const MaterialShaderData& matData = materials.getMaterialShaderData(model.getBatchMaterialName(i));
        uint texWidth, texHeight;
        gfxDevice.getTextureSize(matData.GetAlbedoMap(), &texWidth, &texHeight);
    }
}
