#pragma once
#include <stdint.h>
#include <map>

#include "renderer/ResourceManagerConfig.h"

namespace GCL
{


class Resource;
class ResourceManager
{
public:
	const Resource *LoadResource(const char *resourceName) ;
	void ReleaseResource(const Resource *resource) ;

	virtual Resource *Allocate(const char *filename)=0;
	virtual void Free(Resource *resource)=0;

protected:
	typedef std::map<uint32_t, Resource*> ResourceCache;
	ResourceCache mResourceCache;
	virtual ~ResourceManager()
	{
	}
};
}
