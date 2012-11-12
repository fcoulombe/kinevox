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

#include "renderer/PixelBufferHAL.h"
#include <gcl/SafeCast.h>
#include "renderer/PixelBufferHAL.h"
#include "renderer/TextureResource.h"
#include "renderer/TextureResourceManager.h"

using namespace GCL;


static const GLenum BytePerPixel[] =
{
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_RGB8,
		GL_RGBA8
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

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
	glDeleteTextures(1, &mTextureId); glErrorCheck();
	if (mTextureResource)
		TextureResourceManager::Instance().ReleaseResource(mTextureResource);

}

Texture::Texture(const PixelBuffer &buffer)
: mTextureId(-1)
{
	mTextureResource = NULL;
	mTextureData.width = buffer.mWidth;;
	mTextureData.height= buffer.mHeight;
	mTextureData.bytesPerPixel = buffer.mBytesPerPixel;

	Initialize(buffer);

}

Texture::Texture(const char *filename)
: mTextureId(-1)
{
	mTextureResource = NULL;
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

	const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
	const PixelBuffer &imageData = tempTextureData.imageData;
	Initialize(imageData);
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
	mTextureData.width = width;
	mTextureData.height= height;
	mTextureData.bytesPerPixel = bytesPerPixel;
	mTextureResource = NULL;
	switch (bytesPerPixel)
	{
	case 1:
	{
		PixelBuffer buffer((PixelMono*)NULL, width, height);
		Initialize(buffer);
		break;
	}
	case 3:
	{
		PixelBuffer buffer((PixelRGB*)NULL, width, height);
		Initialize(buffer);
		break;
	}
	case 4:
	{
		PixelBuffer buffer((PixelRGBA*)NULL, width, height);
		Initialize(buffer);
		break;
	}
	}
}

void Texture::Initialize(const PixelBuffer &data )
{
	glGenTextures(1, &mTextureId); glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();

	GCLAssert(BytesPerPixel[data.mBytesPerPixel-1]<= GL_RGBA);
	/*glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
			BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();*/
	/*#ifndef ES1
	if (data)
		gluBuild2DMipmaps( GL_TEXTURE_2D, GLUInternalFormat[bytesPerPixel-1],
				width,height, BytesPerPixel[bytesPerPixel-1],
				GL_UNSIGNED_BYTE, data);
	else
#endif*/
#if 0
	glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[data.mBytesPerPixel-1],
			data.mWidth, data.mHeight, 0,BytesPerPixel[data.mBytesPerPixel-1],
			GL_UNSIGNED_BYTE, data.mPixels);glErrorCheck();
	glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
#else
	glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[data.mBytesPerPixel-1],
			data.mWidth, data.mHeight, 0,BytesPerPixel[data.mBytesPerPixel-1],
			GL_UNSIGNED_BYTE, NULL);glErrorCheck();

	mPBO = new PixelBufferHAL(data);
	if (data.mPixels)
	{
		mPBO->Bind();
		mPBO->PushData();
		//push from pbo to texture
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data.mWidth,
				data.mHeight, BytesPerPixel[data.mBytesPerPixel-1],
				GL_UNSIGNED_BYTE, 0);glErrorCheck();

		mPBO->UnBind();
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);glErrorCheck();
#endif
}

bool Texture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);

	const PixelBufferHAL &imageData = mTextureResource->mTextureData.imageData;
	mTextureData.width = imageData.mWidth;
	mTextureData.height= imageData.mHeight;
	mTextureData.bytesPerPixel = imageData.mBytesPerPixel;

	return mTextureResource != 0;
}

const uint8_t *Texture::GetPixelBufferFromVRAM() const
{
	mPBO->Bind();
	uint8_t *buffer = mPBO->PullData();
	mPBO->UnBind();
	return buffer;
}

const uint8_t *Texture::GetTextureFromVRAM() const
{
#if !defined(ES1) && !defined(ES2)
	Bind();
	//long imageSize = mTextureData.GetImageSizeInBytes();
	GLint width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	long imageSize = width*height*mTextureData.bytesPerPixel;
	//GCLAssert(width == (GLint)mTextureData.width);
	//GCLAssert(height == (GLint)mTextureData.height);
	uint8_t *data = new uint8_t[imageSize];
	memset(data, 0, imageSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);glErrorCheck();

	if (mTextureResource)
		glGetTexImage(GL_TEXTURE_2D, 0, mTextureResource->mTextureData.mTextureFormat, GL_UNSIGNED_BYTE, data);
	else
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glErrorCheck();
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
	PixelBuffer::SaveTga(name.c_str(), GetWidth(), GetHeight(), mTextureData.bytesPerPixel,buffer);
	delete [] buffer;
}
