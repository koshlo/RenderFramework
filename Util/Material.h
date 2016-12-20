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
	float3 albedo;
	float roughness;
	float metallic;

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

template <>
Optional<TextureID> RenderResourceLoader::SyncLoad(const TextureResId& resId) const;


#endif // _MATERIAL_H_