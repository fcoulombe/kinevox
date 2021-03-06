/*
 * Copyright (C) 2014 by Francois Coulombe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "renderer/GL/GLShaderResourceManager.h"
#include "renderer/GL/GLShaderResource.h"
#include "renderer/ShaderResource.h"

using namespace GCL;

GLShaderResourceManager *GLShaderResourceManager::smpInstance = NULL;

GPUResource * GLShaderResourceManager::Allocate( const Resource *resource )
{
  return new GLShaderResource(static_cast<const ShaderResource*>(resource));
}

void GLShaderResourceManager::Free( GPUResource * resource )
{
	delete resource;
}

GLGPUProgramResource * GCL::GLShaderResourceManager::LoadProgramResource(const GLShaderResource *pshader, const GLShaderResource *vshader )
{
	uint8_t pid = pshader->GetShaderObject() & 0x000000ff;
	uint8_t vid = vshader->GetShaderObject() & 0x000000ff;
	uint64_t key = (pid<<8) | vid;
	auto it = mProgramResourceCache.find(key);
	if (it != mProgramResourceCache.end())
	{
		++it->second->mRefCount;
		return it->second;
	}
	GLGPUProgramResource *newResource = new GLGPUProgramResource(pshader, vshader);
	mProgramResourceCache[key] = newResource;
	return newResource;
}

void GCL::GLShaderResourceManager::ReleaseProgramResource( GLGPUProgramResource *resource )
{
	for (ProgramResourceCache::iterator it = mProgramResourceCache.begin(); it != mProgramResourceCache.end(); ++it)
	{
		GLGPUProgramResource *tempResource = it->second;
		if (tempResource == resource)
		{
			--tempResource->mRefCount;
			GCLAssert((long)(resource->mRefCount)>=0); //if we get a ref count of -1, something went really wrong;
			if (resource->mRefCount == 0)
			{
				delete tempResource;
				mProgramResourceCache.erase(it);
			}
			return;
		}
	}
	GCLAssertMsg(false, "we released a program that didnt exist.");
}
