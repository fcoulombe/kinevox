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
	RenderPipe::SendCommandSync(RenderCommand(TEXTURE_DESTROY, this));
	TextureResourceManager::Instance().ReleaseResource(mTextureResource);
}

Texture::Texture(const PixelBuffer &buffer)
{
	mTextureResource = TextureResourceManager::Instance().CreateResource(buffer);
	Create();
}

Texture::Texture(const char *filename)
{
	mTextureResource = NULL;
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());
	Create();
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
	mTextureResource = NULL;
	switch (bytesPerPixel)
	{
	case 1:
	{
		mTextureResource = TextureResourceManager::Instance().CreateResource((PixelMono*)NULL, width, height);
		break;
	}
	case 3:
	{
		mTextureResource = TextureResourceManager::Instance().CreateResource((PixelRGB*)NULL, width, height);
		break;
	}
	case 4:
	{
		mTextureResource = TextureResourceManager::Instance().CreateResource((PixelRGBA*)NULL, width, height);

		break;
	}
	}
	Create();
}


void Texture::Create()
{
	RenderPipe::SendCommand(RenderCommand2Arg(TEXTURE_CREATE, this, (void*)mTextureResource));
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
#ifdef ES2
	(void)filename;
#else
	std::string name(filename);
	std::string nameResource(filename);
	nameResource += "res.tga";
	const uint8_t *buffer = GetTextureFromVRAM();
	PixelBuffer::SaveTga(name.c_str(), GetWidth(), GetHeight(), GetBytesPerPixel(),buffer);
	delete [] buffer;
#endif
}



size_t GCL::Texture::GetResourceWidth() const
{
	return mTextureResource->mTextureData.imageData.mWidth;
}

size_t GCL::Texture::GetResourceHeight() const
{
	return  mTextureResource->mTextureData.imageData.mHeight;
}

size_t GCL::Texture::GetResourceBytesPerPixel() const
{
	return mTextureResource->mTextureData.imageData.mBytesPerPixel;
}
