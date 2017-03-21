#ifndef _OBJ_MATERIAL_LIB_H_
#define _OBJ_MATERIAL_LIB_H_

#include <vector>
#include <memory>

#include "Array.h"
#include "ArrayMap.h"
#include "String.h"
#include "Material.h"
#include "Optional.h"
#include "ResourceLoader.h"

class ObjMaterialLib;
class MaterialShaderData;

struct ObjMaterialLibTraits
{
	typedef ObjMaterialLib ReturnType;
};

typedef ResourceId<ObjMaterialLibTraits> MaterialLibResId;

class ObjMaterialLib
{
public:
	typedef Optional<const Material&> MaterialOptRef;

	ObjMaterialLib(const RenderResourceLoader& resourceLoader);
	~ObjMaterialLib();

	ObjMaterialLib(ObjMaterialLib&& other) = default;
	 
	bool load(const MaterialLibResId& matLibId);

	const MaterialShaderData& getMaterialShaderData(const std::string& matName) const;
	MaterialShaderData* getMaterialShaderData(const std::string& matName);

private:
	struct MaterialDataHolder;

	typedef ArrayMap<MaterialResId, Material> MaterialMap;
	typedef ArrayMap<TextureResId, TextureID> TextureMap;

	void loadTexture(const TextureResId& name, uint flags = 0);
	
	template <typename MatType>
	bool loadMaterial(const MaterialLibResId& matLibId);

	template <typename MatType>
	MatType& addMaterial(const ResourceId<MaterialTraits<MatType>>& materialKey);

	TextureID getTexture(const TextureResId& name, TextureID defaultTexture = TEXTURE_NONE) const;
	const MaterialOptRef getMaterial(const MaterialResId& name) const;

	const RenderResourceLoader& _resourceLoader;
	MaterialMap _materialMap;
	TextureMap _loadedTextures;

	std::unique_ptr<MaterialDataHolder> _materialDataHolder;
};

template <>
void RenderResourceLoader::SyncLoad(const MaterialLibResId& matLibId, Optional<ObjMaterialLib>* resultLib) const;

#endif
