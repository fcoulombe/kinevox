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

#include <cstdlib>
#include <gcl/Matrix44.h>
#include <gcl/Point3.h>
#include "renderer/Texture.h"
#include "renderer/TTFFont.h"
#include "renderer/RenderObject.h"
namespace GCL
{
class Text2D
{
public:
	Text2D(const char *text);
	virtual ~Text2D()
	{
		delete mObj;
		delete mTexture;
	}

	void Update()
	{
	}
	void Render(const Matrix44 &projection);
	void SetVisible(bool isVisible = true) { mIsVisible = isVisible; }
	bool IsVisible() const { return mIsVisible; }

	size_t GetWidth() const { return buffer.mWidth; }
	size_t GetHeight() const { return buffer.mHeight; }

	void SetPosition(const WorldPoint3 &position)  	{mTransform.SetPosition(position);	}
	const WorldPoint3 &GetPosition() const { return *(const WorldPoint3 *)&mTransform.GetPosition(); }

	void SetScale(const WorldPoint2 &scale) { mScale = scale; }
	const WorldPoint2 &GetScale() const { return mScale; }

	void Save(const char *filename) { mTexture->Save(filename);}
	operator RenderObject *() { return mObj; }
protected:
	Matrix44 mTransform;
	TTFFont mFont;
	Texture *mTexture;
	RenderObject *mObj;
	bool mIsVisible;
	Material mMaterial;
	WorldPoint2 mScale;
	PixelBuffer buffer;
	VertexP square[6];
};
}
