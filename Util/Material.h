#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "../Math/Vector.h"
#include "../Renderer.h"
#include "ResourceLoader.h"

struct TextureResTrait
{
	typedef TextureID ReturnType;
};

typedef ResourceId<TextureResTrait> TextureResId;

struct Material
{
	Material() : ambient(0), diffuse(0), specular(0), specExponent(0) {}

	float3 ambient;
	float3 diffuse;
	float3 specular;
	float specExponent;

	TextureResId ambientMap;
	TextureResId diffuseMap;
	TextureResId bumpMap;
};

struct PBRMaterial
{
	TextureResId albedoMap;
	TextureResId normalMap;
	TextureResId roughnessMap;
	TextureResId metallicMap;
};

template <typename MatType>
struct MaterialTraits
{
	typedef MatType ReturnType;
};

typedef ResourceId<MaterialTraits<Material>> MaterialResId;
typedef ResourceId<MaterialTraits<PBRMaterial>> PBRMaterialResId;

template <>
Optional<TextureID> RenderResourceLoader::SyncLoad(const TextureResId& resId);


#endif // _MATERIAL_H_