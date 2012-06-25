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

#include "renderer/Sprite.h"
#include <gcl/Assert.h>
#include <gcl/File.h>
#include <gcl/ResourceManagerConfig.h>
#include "renderer/Texture.h"

#include <fstream>

using namespace GCL;

Sprite::~Sprite()
{
	for (size_t i=0; i<mTextureList.size(); ++i)
	{
		Texture *tempTexture = mTextureList[i];
		delete tempTexture;
	}
	mTextureList.clear();
}

Sprite::Sprite(const char *filename)
: mCurrentFrame(0),
  mScale(1.0,1.0),
  mIsPlaying(false)
{
	LoadSprite(filename);
}


void Sprite::LoadSprite(const char * filename)
{
	const std::string fullFileName(std::string(SPRITE_PATH) + std::string(filename) + std::string(".spr"));

	GCLFile fp(fullFileName.c_str());

	size_t fileSize = fp.GetFileSize();
	mSpriteData = new uint8_t[fileSize];
	fp.Read((char*)mSpriteData, fileSize);
	mHeader = (SpriteDataHeader*)mSpriteData;

	uint8_t *currentPtr = mSpriteData+sizeof(SpriteDataHeader);
	for (size_t i=0; i<mHeader->textureCount; ++i)
	{
		uint32_t strLen = *(uint32_t*)currentPtr;
		currentPtr += sizeof(uint32_t);

		const char *filename = (const char *)currentPtr;
		currentPtr += strLen+1;

		std::string fullTextureFileName(TEXTURE_PATH);
		fullTextureFileName += filename;
		Texture *texture = new Texture(fullTextureFileName.c_str());

		GCLAssertMsg(mHeader->width <= texture->GetWidth(), std::string(filename) + ": You have a sprite that is bigger than your texture");
		GCLAssertMsg(mHeader->height <= texture->GetHeight(), std::string(filename) + ": You have a sprite that is bigger than your texture");

		mTextureList.push_back(texture);
	}
}

void Sprite::Play()
{
	mIsPlaying = true;
}

void Sprite::Pause()
{
	mIsPlaying = false;
}

void Sprite::Rewind()
{
	mCurrentFrame = 0;
}


void Sprite::Update()
{
	++mCurrentFrame;
	if (mCurrentFrame>=mHeader->frameCount)
		mCurrentFrame = 0;
}

void Sprite::Render() const
{
	GCLAssert(mTextureList.size());
	//figure out what is the texture coordinate for our current frame

	const Texture &firstTexture = *(mTextureList[0]);

	//we use the size of the first texture to detemrine everything else
	size_t width = firstTexture.GetWidth();
	size_t height = firstTexture.GetHeight();
	size_t framePerWidth = width/mHeader->width;
	size_t rowPerTexture = height/mHeader->height;

	//size_t offset = mHeader.width*mCurrentFrame;
	size_t whatRow = mCurrentFrame/framePerWidth;
	size_t whatCol = mCurrentFrame%framePerWidth;
	size_t whatTexture = whatRow/rowPerTexture;
	//adjust the column to offset in the texture
	whatRow = whatRow- (whatTexture*rowPerTexture);

	WorldPoint2 topTextureCoord, bottomTextureCoord;
	Real widthRatio = mHeader->width/Real(width);
	Real heightRatio = mHeader->height/Real(height);
	topTextureCoord.x = whatCol*widthRatio;
	topTextureCoord.y = whatRow*heightRatio;
	bottomTextureCoord.x = topTextureCoord.x +widthRatio;
	bottomTextureCoord.y = topTextureCoord.y +heightRatio;

	//std::cout << "top " << topTextureCoord << std::endl;
	//std::cout << "bot " <<bottomTextureCoord << std::endl;

	mTextureList[whatTexture]->Bind();
	glBegin (GL_TRIANGLE_STRIP);
#if 1
	Real halfWidth = (width/2.0)*mScale.x;
	Real halfHeight = (height/2.0)*mScale.y;

	glTexCoord2f (topTextureCoord.x, topTextureCoord.y);
	glVertex3f (-halfWidth+mPosition.x, -halfHeight+mPosition.y, 0.0);
	glTexCoord2f (bottomTextureCoord.x, topTextureCoord.y);
	glVertex3f (halfWidth+mPosition.x, -halfHeight+mPosition.y, 0.0);
	glTexCoord2f (topTextureCoord.x, bottomTextureCoord.y);
	glVertex3f (-halfWidth+mPosition.x, halfHeight+mPosition.y, 0.0);
	glTexCoord2f (bottomTextureCoord.x, bottomTextureCoord.y);
	glVertex3f (halfWidth+mPosition.x, halfHeight+mPosition.y, 0.0);
#else
	glTexCoord2f (0.0, 0.0);
	glVertex3f (-5, -5, 0.0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f (5, -5, 0.0);
	glTexCoord2f (0.0, 1.0);
	glVertex3f (-5, 5, 0.0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f (5, 5, 0.0);

#endif
	glEnd ();

}
