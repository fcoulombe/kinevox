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

#include "renderer/Texture.h"

#include <cstring>
#include <stdint.h>

#include <gcl/PixelBuffer.h>
#include <gcl/SafeCast.h>
#include "renderer/TextureResource.h"
#include "renderer/TextureResourceManager.h"

using namespace GCL;

#include <iostream>
Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
	glDeleteTextures(1, &mTextureId); glErrorCheck();
	if (mTextureResource)
		TextureResourceManager::Instance().ReleaseResource(mTextureResource);
}

Texture::Texture(const char *filename)
: mTextureId(-1)
{
	mTextureResource = NULL;
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

	const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
	Initialize(tempTextureData.mWidth, tempTextureData.mHeight, tempTextureData.mBytePerPixel, tempTextureData.imageData);
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
	mTextureData.width = width;
	mTextureData.height= height;
	mTextureData.bytesPerPixel = bytesPerPixel;
	mTextureResource = NULL;
	Initialize(width, height, bytesPerPixel);
}
static const GLenum BytePerPixel[] =
{
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_RGB,
		GL_RGBA
};
static const GLenum BytesPerPixel[] =
{
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_RGB,
		GL_RGBA,
};
static const GLenum GLUInternalFormat[] =
{
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_RGB,
		GL_RGBA
};
void Texture::Initialize(size_t width, size_t height, size_t bytesPerPixel, const uint8_t *data )
{
	glGenTextures(1, &mTextureId); glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();

	GCLAssert(bytesPerPixel <= GL_RGBA);
	/*glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
			BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();*/
#ifndef ES1
	if (data)
		gluBuild2DMipmaps( GL_TEXTURE_2D, GLUInternalFormat[bytesPerPixel-1],
				width,height, BytesPerPixel[bytesPerPixel-1],
				GL_UNSIGNED_BYTE, data);
	else
#endif
		glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
				BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();

	glBindTexture(GL_TEXTURE_2D, 0);glErrorCheck();
}

bool Texture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);


	mTextureData.width = mTextureResource->mTextureData.mWidth;
	mTextureData.height= mTextureResource->mTextureData.mHeight;
	mTextureData.bytesPerPixel = mTextureResource->mTextureData.mBytePerPixel;

	return mTextureResource != 0;
}



const uint8_t *Texture::GetTextureFromVRAM() const
{
#if !defined(ES1) && !defined(ES2)
	Bind();
	long imageSize = mTextureData.GetImageSizeInBytes();
	uint8_t *data = new uint8_t[imageSize];

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	if (mTextureResource)
		glGetTexImage(GL_TEXTURE_2D, 0, mTextureResource->mTextureData.mTextureFormat, GL_UNSIGNED_BYTE, data);
	else
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	return data;
#else
	GCLAssert(false && "unsupported");
	return NULL;
#endif
}


//TODO: push this in the texture resource
void Texture::Save(const char *filename)
{
	std::string name(filename);
	std::string nameResource(filename);
	nameResource += "res.tga";
	const uint8_t *buffer = GetTextureFromVRAM();
	PixelBuffer::SaveTga(name.c_str(), mTextureData.width, mTextureData.height, mTextureData.bytesPerPixel,buffer);
	delete [] buffer;
}
