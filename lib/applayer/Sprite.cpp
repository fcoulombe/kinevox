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

#include "applayer/Sprite.h"
#include <sstream>
#include "applayer/GCLApplication.h"
#include <gcl/Assert.h>
#include <gcl/File.h>

#include <renderer/Material.h>
#include <renderer/RenderObject.h>
#include <renderer/Texture.h>

#include <script/ConfigLua.h>

using namespace GCL;
Sprite::~Sprite()
{

	GCLApplication::ReleaseSprite(this);
	delete mObj;
	delete mMaterial;
	for (size_t i=0; i<mTextureList.size(); ++i)
	{
		Texture *tempTexture = mTextureList[i];
		delete tempTexture;
	}
	mTextureList.clear();
}

Sprite::Sprite(const char *filename)
: mCurrentFrame(0),
  mIsPlaying(false),
  mScale(1.0, 1.0)

{
	LoadSprite(filename);
	GCLApplication::RegisterSprite(this);
}


void Sprite::LoadSprite(const char * filename)
{
	const std::string fullFileName(std::string(SPRITE_PATH) + std::string(filename) + std::string(".spr"));
	ConfigLua conf(fullFileName.c_str());
	

	mHeader.frameCount = conf.GetInt("gclsprite.frame_count");
	mHeader.width = conf.GetInt("gclsprite.width");
	mHeader.height = conf.GetInt("gclsprite.height");
	mHeader.textureCount = conf.GetInt("gclsprite.texture_count");
	std::stringstream fullTextureKey;
	const std::string textureKey("gclsprite.textures.texture");
	for (size_t i=0; i<mHeader.textureCount; ++i)
	{
		fullTextureKey.str("");
		fullTextureKey << textureKey;
		fullTextureKey << i;
		const std::string filename = conf.GetString(fullTextureKey.str().c_str());
		std::string fullTextureFileName(TEXTURE_PATH);
		fullTextureFileName += filename;
		Texture *texture = new Texture(fullTextureFileName.c_str());

		GCLAssertMsg(mHeader.width <= texture->GetWidth(), std::string(filename) + ": You have a sprite that is bigger than your texture");
		GCLAssertMsg(mHeader.height <= texture->GetHeight(), std::string(filename) + ": You have a sprite that is bigger than your texture");

		mTextureList.push_back(texture);
	}

	MeshReal halfWidth = MeshReal(mHeader.width/2.0);
	MeshReal halfHeight = MeshReal(mHeader.height/2.0);

	const   VertexPT square[6] = {
		{Point3<MeshReal>(-halfWidth, -halfHeight, 0.0),Point2<MeshReal>(0.0, 0.0)},
		{Point3<MeshReal>(halfWidth, -halfHeight, 0.0), Point2<MeshReal>(1.0, 0.0)},
		{Point3<MeshReal>(-halfWidth, halfHeight, 0.0), Point2<MeshReal>(0.0, 1.0)},
		{Point3<MeshReal>(-halfWidth, halfHeight, 0.0), Point2<MeshReal>(0.0, 1.0)},
		{Point3<MeshReal>(halfWidth, -halfHeight, 0.0), Point2<MeshReal>(1.0, 0.0)},
		{Point3<MeshReal>(halfWidth, halfHeight, 0.0), Point2<MeshReal>(1.0, 1.0)}
	};

	mMaterial = new Material("DefaultSprite");
	mObj = new RenderObject(Matrix44::IDENTITY, *mMaterial, square, 6);

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
	if (!IsPlaying())
		return;
	++mCurrentFrame;
	if (mCurrentFrame>=mHeader.frameCount)
		mCurrentFrame = 0;
}


void Sprite::Render(const Matrix44 &viewMatrix)
{
	const RenderObject *tempRenderObject = mObj;
	const Material &tempMaterial = tempRenderObject->GetMaterial();
	tempMaterial.Bind();
	const Matrix44 &transform = tempRenderObject->GetTransform();
	GPUProgram *tempProgram = tempMaterial.GetShader();
	tempProgram->SetProjectionMatrix(viewMatrix);
	tempProgram->SetModelViewMatrix(Matrix44::IDENTITY*transform);
	tempProgram->SetUniform("CurrentFrame", (int)mCurrentFrame);
	tempProgram->SetUniform("Dimension", Point2<int>(int(mHeader.width), int(mHeader.height)));
	tempProgram->SetUniform("FrameCount", (int)mHeader.frameCount);
	tempProgram->SetUniform("TextureSize", Point2<int>((int)mTextureList[0]->GetResourceWidth(), (int)mTextureList[0]->GetResourceHeight()));
	tempProgram->SetTextureSampler(*mTextureList[0]);
	mTextureList[0]->Bind();
	tempRenderObject->GetVBO().Render();
}
#if 0
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
	topTextureCoord.y = 1.0-whatRow*heightRatio;
	bottomTextureCoord.x = topTextureCoord.x +widthRatio;
	bottomTextureCoord.y = topTextureCoord.y -heightRatio;

	//std::cout << "top " << topTextureCoord << std::endl;
	//std::cout << "bot " <<bottomTextureCoord << std::endl;

	mTextureList[whatTexture]->Bind();

    Real halfWidth = (mHeader->width/2.0)*mScale.x;
    Real halfHeight = (mHeader->height/2.0)*mScale.y;
    VertexPT square[4];
    square[0].position = Point3<MeshReal>(MeshReal(halfWidth+mPosition.x), MeshReal(-halfHeight+mPosition.y), MeshReal(0.0));
    square[0].textureCoordinate = Point2<MeshReal>(MeshReal(bottomTextureCoord.x), MeshReal(topTextureCoord.y));

    square[1].position =Point3<MeshReal>(MeshReal(-halfWidth+mPosition.x), MeshReal(-halfHeight+mPosition.y), MeshReal(0.0));
    square[1].textureCoordinate = Point2<MeshReal>(MeshReal(topTextureCoord.x), MeshReal(topTextureCoord.y));

    square[2].position = Point3<MeshReal>(MeshReal(halfWidth+mPosition.x), MeshReal(halfHeight+mPosition.y), MeshReal(0.0));
    square[2].textureCoordinate = Point2<MeshReal>(MeshReal(bottomTextureCoord.x), MeshReal(bottomTextureCoord.y));

    square[3].position = Point3<MeshReal>(MeshReal(-halfWidth+mPosition.x), MeshReal(halfHeight+mPosition.y), MeshReal(0.0));
    square[3].textureCoordinate = Point2<MeshReal>(MeshReal(topTextureCoord.x), MeshReal(bottomTextureCoord.y));
#if 0
	VertexBuffer buffer(square, 4);
    buffer.PreRender();
    buffer.Render(VBM_TRIANGLE_STRIP);
    buffer.PostRender();
#else
	GCLAssert(false && "reimplement");
#endif
}
#endif

void GCL::Sprite::SetPosition( const WorldPoint3 &position )
{
	mObj->SetPosition(position);
}

const WorldPoint3 & GCL::Sprite::GetPosition() const
{
	return *(const WorldPoint3 *)&mObj->GetTransform().GetPosition();
}
