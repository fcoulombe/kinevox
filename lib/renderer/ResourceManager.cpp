#include "renderer/ResourceManager.h"

#include <gcl/Hash.h>
#include <gcl/Assert.h>
#include "renderer/Resource.h"

using namespace GCL;


const Resource * ResourceManager::LoadResource( const char *textureName )
{
  //hash the name for faster search in the tree
  uint32_t fileNameHash = Hash::DJB(textureName, strlen(textureName));

  //check if we have the resource in our ResourceManager
  ResourceCache::iterator it = mResourceCache.find(fileNameHash);
  if (it != mResourceCache.end())
    {
      ++it->second->mRefCount;
      return it->second;
    }

  //if we don't then load the resource
  Resource *newResource = Allocate(textureName);
  mResourceCache[fileNameHash] = newResource;
  return newResource;
}

//reduce the ref count
void ResourceManager::ReleaseResource( const Resource *resource )
{
  for (ResourceCache::iterator it = mResourceCache.begin(); it != mResourceCache.end(); ++it)
    {
      if (it->second == resource)
        {
          --it->second->mRefCount;
          GCLAssert((ssize_t)(resource->mRefCount)>=0); //if we get a ref count of -1, something went really wrong;
          return;
        }
    }
  GCLAssertMsg(false, "we released a texture that didnt exist.");
}

