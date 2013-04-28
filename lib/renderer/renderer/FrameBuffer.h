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

#define FRAMEBUFFER_INCLUDE GET_GFX_INCLUDE(FrameBuffer.h)
#define IFrameBuffer GET_GFX_CLASS(FrameBuffer)
#include FRAMEBUFFER_INCLUDE

namespace GCL
{

class Texture;
class RenderBuffer;
class FrameBuffer
{
public:
	FrameBuffer(const Texture & texture, const RenderBuffer & depthBuffer);
	~FrameBuffer()
	{
	}
	void Bind()
	{
		mPimpl.Bind();
	}
	static void ResetDefault()
	{
		IFrameBuffer::ResetDefault();
	}

	bool IsValid() const { return mPimpl.IsValid(); }

	void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }
private:
	IFrameBuffer mPimpl;
};

}
