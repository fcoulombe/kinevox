/*
 * Copyright (C) 2013 by Francois Coulombe
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

#include "renderer/Texture.h"

#include <cstring>
#include <stdint.h>

#include <gcl/SafeCast.h>
#include "renderer/TextureResource.h"
#include "renderer/TextureResourceManager.h"

using namespace GCL;

Texture::~Texture()
{
	delete mPimpl;
	if (mTextureResource)
		TextureResourceManager::Instance().ReleaseResource(mTextureResource);
}

Texture::Texture(const PixelBuffer &buffer)
{
	mTextureResource = NULL;
	mPimpl = new ITexture(buffer);
}

Texture::Texture(const char *filename)
{
	mTextureResource = NULL;
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

	const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
	const PixelBuffer &imageData = tempTextureData.imageData;

	mPimpl = new ITexture(imageData);
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
	mTextureResource = NULL;
	switch (bytesPerPixel)
	{
	case 1:
	{
		PixelBuffer buffer((PixelMono*)NULL, width, height);
        mPimpl = new ITexture(buffer);
		break;
	}
	case 3:
	{
		PixelBuffer buffer((PixelRGB*)NULL, width, height);
		mPimpl = new ITexture(buffer);
		break;
	}
	case 4:
	{
		PixelBuffer buffer((PixelRGBA*)NULL, width, height);
        mPimpl = new ITexture(buffer);
		break;
	}
	}
}



bool Texture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);
	return mTextureResource != 0;
}




//TODO: push this in the texture resource
void Texture::Save(const char *filename)
{
	std::string name(filename);
	std::string nameResource(filename);
	nameResource += "res.tga";
	const uint8_t *buffer = GetTextureFromVRAM();
	PixelBuffer::SaveTga(name.c_str(), GetWidth(), GetHeight(), GetBytesPerPixel(),buffer);
	delete [] buffer;
}


