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

#include <vector>

#include "renderer/Material.h"

namespace GCL
{
struct SpriteDataHeader
{
	uint16_t width;
	uint16_t height;
	uint32_t frameCount;
	uint32_t textureCount;
};

class Texture;
class Sprite
{
public:
	Sprite(const char *filename="DefaultSprite");

	void Render() const;

	void Update();
	void Play();
	void Pause();
	void Rewind();

	size_t GetWidth() const { return mHeader.width; }
	size_t GetHeight() const { return mHeader.height; }
	size_t GetFrameCount() const { return mHeader.frameCount; }

	void SetPosition(const WorldPoint3 &position)  	{mPosition = position;	}
	const WorldPoint3 &GetPosition() const { return mPosition; }

	void SetScale(const WorldPoint2 &scale) { mScale = scale; }
private:
	void LoadSprite(const char *filename);
	std::vector<Texture*> mTextureList;

	SpriteDataHeader mHeader;

	size_t mCurrentFrame;
	WorldPoint3 mPosition;
	WorldPoint2 mScale;
	bool mIsPlaying;
};

}
