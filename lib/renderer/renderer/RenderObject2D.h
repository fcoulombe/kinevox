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

#include "renderer/Sprite.h"

namespace GCL
{

class RenderObject2D
{
public:
	RenderObject2D()
	: mObjName("Un-Init"),
	  mIsVisible(true)
	{
		mSprite = NULL;
	}
	RenderObject2D(const char *name, const char *spriteFilename)
	: mObjName(name),
	  mIsVisible(true)
	{
		mSprite = new Sprite(spriteFilename);
	}
	virtual ~RenderObject2D()
	{
		if (mSprite)
			delete mSprite;
	}

	void Update()
	{
		mSprite->Update();
	}
	void Render()
	{
		if (mIsVisible)
			mSprite->Render();
	}

	void SetVisible(bool isVisible = true) { mIsVisible = isVisible; }
	bool IsVisible() const { return mIsVisible; }

	void SetPosition(Real x, Real y,Real z)
	{ SetPosition(WorldPoint3(x,y,z));	}
	void SetPosition(const WorldPoint3 &position)
	{ mSprite->SetPosition(position); }
	const WorldPoint3 &GetPosition() const { return mSprite->GetPosition(); }

	void SetScale(const WorldPoint2 &scale) { mSprite->SetScale(scale); }

	void SetSprite(const char *filename)
	{
		if (mSprite)
			delete mSprite;
		mSprite = new Sprite(filename);
	}

	void SetName(const char *name) { mObjName = name; }

	size_t GetWidth() const { return mSprite->GetWidth(); }
	size_t GetHeight() const { return mSprite->GetHeight(); }

	size_t GetScaledWidth() const { return mSprite->GetWidth()*mSprite->GetScale().x; }
	size_t GetScaledHeight() const { return mSprite->GetHeight()*mSprite->GetScale().y; }
protected:
	Sprite *mSprite;
	std::string mObjName;
	bool mIsVisible;
};
}
