/*
 * Copyright (C) 2011 by Francois Coulombe
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

#pragma once
#include <sstream>
#include <gcl/Assert.h>
#include <gcl/Hash.h>
#include <gcl/ResourceManager.h>
#include "renderer/TextureResource.h"

namespace GCL
{
  //very simple resource manager for texture
  //when something asks it to load a texture, it will hash the name and then
  //look in the map if we already have this texture loaded. if its the case, it will return
  //a reference and increase the ref count. this is so that we don't waste memory loading similar textures
  class TextureResource;
  class TextureResourceManager : public ResourceManager
  {
  public:
    static void Initialize();
    static void Terminate();
    static TextureResourceManager &Instance() {	GCLAssert(smpInstance != NULL);return *smpInstance;}

    Resource *Allocate(const char *filename);
    void Free(Resource * /*resource*/);

	template<typename PixelType>
	TextureResource *CreateResource(PixelType *pixelData, size_t width, size_t height)
	{
		TextureResource *newTextureResource = new TextureResource(pixelData, width, height);
		
		++createdTextureResourceCount;
		std::stringstream fileName;
		fileName << "CreatedTextureResource" << createdTextureResourceCount;
		uint32_t key = Hash::DJB(fileName.str().c_str(), fileName.str().length());
		mResourceCache[key] = newTextureResource;
		return newTextureResource;
	}
	TextureResource *CreateResource(const PixelBuffer &pixelbuffer)
	{
		TextureResource *newTextureResource = new TextureResource(pixelbuffer);

		++createdTextureResourceCount;
		std::stringstream fileName;
		fileName << "CreatedTextureResource" << createdTextureResourceCount;
		uint32_t key = Hash::DJB(fileName.str().c_str(), fileName.str().length());
		mResourceCache[key] = newTextureResource;
		return newTextureResource;
	}
  private:
    static TextureResourceManager *smpInstance;
	static size_t createdTextureResourceCount;

    TextureResourceManager() {}
  };

}

