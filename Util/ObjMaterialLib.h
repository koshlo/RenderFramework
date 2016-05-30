#ifndef _OBJ_MATERIAL_LIB_H_
#define _OBJ_MATERIAL_LIB_H_

#include <vector>

#include "Array.h"
#include "ArrayMap.h"
#include "String.h"
#include "Material.h"
#include "Optional.h"
#include "ResourceLoader.h"

class ObjMaterialLib;

struct ObjMaterialLibTraits
{
	typedef ObjMaterialLib ReturnType;
	typedef bool InfoType;
};

typedef ResourceId<ObjMaterialLibTraits> MaterialLibResId;

class ObjMaterialLib
{
public:
	typedef Optional<const Material&> MaterialOptRef;
	typedef Optional<const PBRMaterial&> PBRMaterialOptRef;

	ObjMaterialLib(RenderResourceLoader& resourceLoader) : _resourceLoader(resourceLoader) {}
	
	bool load(const MaterialLibResId& matLibId, bool isPbr);

	void applyMaterial(const std::string& name, Renderer* renderer);

private:
	typedef ArrayMap<MaterialResId, Material> MaterialMap;
	typedef ArrayMap<PBRMaterialResId, PBRMaterial> PBRMaterialMap;
	typedef ArrayMap<TextureResId, TextureID> TextureMap;

	void loadTexture(const TextureResId& name);
	
	template <typename MatType>
	bool loadMaterial(const MaterialLibResId& matLibId);

	template <typename MatType>
	MatType& addMaterial(const ResourceId<MaterialTraits<MatType>>& materialKey);

	void applyMaterial(const MaterialResId& matId, Renderer* renderer);
	void applyMaterial(const PBRMaterialResId& matId, Renderer* renderer);

	const MaterialOptRef getMaterial(const MaterialResId& name) const;
	const PBRMaterialOptRef getMaterial(const PBRMaterialResId& name) const;
	Optional<TextureID> getTexture(const TextureResId& name) const;

	RenderResourceLoader _resourceLoader;
	MaterialMap _materialMap;
	PBRMaterialMap _pbrMaterialMap;
	TextureMap _loadedTextures;
	bool _isPBR;
};

template <>
void RenderResourceLoader::SyncLoad(const MaterialLibResId& matLibId, Optional<ObjMaterialLib>* resultLib);

#endif
