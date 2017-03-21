#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_

#include "../Renderer.h"
#include "Resource.h"
#include "Optional.h"

class RenderResourceLoader
{
public:
	explicit RenderResourceLoader(GraphicsDevice& gfxDevice) : _device(gfxDevice) {};

	template <typename ResTrait>
	Optional<typename ResTrait::ReturnType> SyncLoad(const ResourceId<ResTrait>& resId) const;

	template <typename ResTrait>
	Optional<typename ResTrait::ReturnType> SyncLoad(const ResourceId<ResTrait>& resId, typename ResTrait::UserData data) const;

	template <typename ResTrait>
	void SyncLoad(const ResourceId<ResTrait>& resId, Optional<typename ResTrait::ReturnType>* result) const;

	template <typename ResTrait>
	void SyncLoad(const ResourceId<ResTrait>& resId, typename ResTrait::UserData data, Optional<typename ResTrait::ReturnType>* result) const;

	static const char* DataPath();
	
	GraphicsDevice& GetGraphicsDevice() const { return _device; }
private:
	GraphicsDevice& _device;
};

#endif // _RESOURCE_LOADER_H_