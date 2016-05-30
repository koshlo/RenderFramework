#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_

#include "../Renderer.h"
#include "Resource.h"
#include "Optional.h"

class RenderResourceLoader
{
public:
	explicit RenderResourceLoader(Renderer& renderer) : _renderer(renderer) {};

	template <typename ResTrait>
	Optional<typename ResTrait::ReturnType> SyncLoad(const ResourceId<ResTrait>& resId);

	template <typename ResTrait>
	Optional<typename ResTrait::ReturnType> SyncLoad(const ResourceId<ResTrait>& resId, typename ResTrait::InfoType info);

	template <typename ResTrait>
	void SyncLoad(const ResourceId<ResTrait>& resId, Optional<typename ResTrait::ReturnType>* result);

	template <typename ResTrait>
	void SyncLoad(const ResourceId<ResTrait>& resId, typename ResTrait::InfoType info, Optional<typename ResTrait::ReturnType>* result);

	static const char* DataPath();

private:
	Renderer& _renderer;
};

#endif // _RESOURCE_LOADER_H_