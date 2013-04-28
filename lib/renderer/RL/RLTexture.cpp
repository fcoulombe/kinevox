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

#include "renderer/RLTexture.h"

#include <cstring>
#include <stdint.h>

#include "renderer/RLPixelBufferHAL.h"
#include <gcl/SafeCast.h>
#include "renderer/TextureResource.h"
#include "renderer/TextureResourceManager.h"

using namespace GCL;



static const RLenum RLBytesPerPixel[] =
{
		RL_LUMINANCE,
		-1,
		RL_RGB,
		RL_RGBA,
};


RLTexture::~RLTexture()
{
	rlBindTexture(RL_TEXTURE_2D, 0);  
	rlDeleteTextures(1, &mTextureId); 
	if (mTextureResource)
		TextureResourceManager::Instance().ReleaseResource(mTextureResource);

}

RLTexture::RLTexture(const PixelBuffer &buffer)
: mTextureId((RLtexture)-1)
{
	mTextureResource = NULL;
	mTextureData.width = buffer.mWidth;;
	mTextureData.height= buffer.mHeight;
	mTextureData.bytesPerPixel = buffer.mBytesPerPixel;

	Initialize(buffer);

}

RLTexture::RLTexture(const char *filename)
    : mTextureId((RLtexture)-1)
{
	mTextureResource = NULL;
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

	const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
	const PixelBuffer &imageData = tempTextureData.imageData;
	Initialize(imageData);
}

RLTexture::RLTexture(size_t width, size_t height, size_t bytesPerPixel )
    : mTextureId((RLtexture)-1)
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

void RLTexture::Initialize(const PixelBuffer &data )
{
	rlGenTextures(1, &mTextureId); 
	rlBindTexture(RL_TEXTURE_2D, mTextureId);

	rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MAG_FILTER, RL_LINEAR);
	rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_MIN_FILTER, RL_LINEAR_MIPMAP_LINEAR);
	rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_S, RL_CLAMP_TO_EDGE);
	rlTexParameteri(RL_TEXTURE_2D, RL_TEXTURE_WRAP_T, RL_CLAMP_TO_EDGE);
	//rlTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();

	GCLAssert(BytesPerPixel[data.mBytesPerPixel-1]<= RL_RGBA);

	rlTexImage2D(RL_TEXTURE_2D, 0, RLBytesPerPixel[data.mBytesPerPixel-1],
			(RLint)data.mWidth, (RLint)data.mHeight, 0,RLBytesPerPixel[data.mBytesPerPixel-1],
			RL_UNSIGNED_BYTE, NULL);

	mPBO = new RLPixelBufferHAL(data);

    mPBO->BindUnPack();
    mPBO->PushData();
    //push from pbo to texture
    rlTexImage2D(RL_TEXTURE_2D, 0, RLBytesPerPixel[data.mBytesPerPixel-1], 
        (RLint)data.mWidth,	(RLint)data.mHeight, 0, RLBytesPerPixel[data.mBytesPerPixel-1],
        RL_UNSIGNED_BYTE, NULL);

    mPBO->UnBindUnPack();

	rlGenerateMipmap(RL_TEXTURE_2D);

}

bool RLTexture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);

	const PixelBuffer &imageData = mTextureResource->mTextureData.imageData;
	mTextureData.width = imageData.mWidth;
	mTextureData.height= imageData.mHeight;
	mTextureData.bytesPerPixel = imageData.mBytesPerPixel;

	return mTextureResource != 0;
}

const uint8_t *RLTexture::GetPixelBufferFromVRAM() const
{
    mPBO->BindPack();
    Bind();
    if (mTextureData.bytesPerPixel == 3)
	    rlGetTexImage(RL_TEXTURE_2D, 0, RL_RGB, RL_UNSIGNED_BYTE, NULL);
    else
        rlGetTexImage(RL_TEXTURE_2D, 0, RL_RGBA, RL_UNSIGNED_BYTE, NULL);
	uint8_t *buffer = mPBO->PullData();
	mPBO->UnBindPack();
	return buffer;
}

const uint8_t *RLTexture::GetTextureFromVRAM() const
{
	GCLAssert(false && "unsupported");
	return NULL;
}


//TODO: push this in the texture resource
void RLTexture::Save(const char *filename)
{
	std::string name(filename);
	std::string nameResource(filename);
	nameResource += "res.tga";
	const uint8_t *buffer = GetPixelBufferFromVRAM();
	PixelBuffer::SaveTga(name.c_str(), GetWidth(), GetHeight(), mTextureData.bytesPerPixel,buffer);
	delete [] buffer;
}
