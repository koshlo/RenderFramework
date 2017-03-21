#include "Material.h"
#include "String.h"

template <>
Optional<TextureID> RenderResourceLoader::SyncLoad(const TextureResId& resId, uint flags) const
{
	String path; path.sprintf("%s%s", DataPath(), resId.getId().c_str());
	TextureID texture = _device.addTexture(path.dataPtr(), true, SS_NONE, flags);
	Optional<TextureID> retTexture;
	if (texture != TEXTURE_NONE)
	{
		retTexture = std::move(texture);
	}
	return retTexture;
}

template <>
Optional<TextureID> RenderResourceLoader::SyncLoad(const TextureResId& resId) const
{
    return SyncLoad(resId, 0u);
}