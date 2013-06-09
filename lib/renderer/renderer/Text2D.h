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
#include <gcl/Point3.h>
#include "renderer/Texture.h"
#include "renderer/TTFFont.h"

namespace GCL
{
class Text2D
{
public:
	Text2D(const char *text);
	virtual ~Text2D()
	{
	}

	void Update()
	{
	}

	void Render();

	void SetVisible(bool isVisible = true) { mIsVisible = isVisible; }
	bool IsVisible() const { return mIsVisible; }

	size_t GetWidth() const { GCLAssert(mTexture);return mTexture->GetWidth(); }
	size_t GetHeight() const { GCLAssert(mTexture);return mTexture->GetHeight(); }

	void SetPosition(const WorldPoint3 &position)  	{mPosition = position;	}
	const WorldPoint3 &GetPosition() const { return mPosition; }

	void SetScale(const WorldPoint2 &scale) { mScale = scale; }
	const WorldPoint2 &GetScale() const { return mScale; }

protected:
	TTFFont mFont;
	Texture *mTexture;
	WorldPoint3 mPosition;
	WorldPoint2 mScale;
	bool mIsVisible;
};
}
