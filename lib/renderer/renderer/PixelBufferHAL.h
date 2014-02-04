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
#include <gcl/PixelBuffer.h>
#include "rendererconf.h"
#include GFXAPI_PixelBufferHAL_H
namespace GCL
{

class PixelBufferHAL : public PixelBuffer
{
public:
	PixelBufferHAL()
	: PixelBuffer()
	{
	}
	PixelBufferHAL(const PixelBuffer &buffer)
	:	PixelBuffer(buffer)
	{
	}

	template<typename PixelType>
	PixelBufferHAL(const PixelType *pixelArray, size_t width, size_t height)
	: PixelBuffer(pixelArray, width, height)
	  {
		Bind();
		mPixelBuffer.PushData(width, height, PixelType::OffsetToNext(), 0);
		UnBind();
	  }

	~PixelBufferHAL()
	{
	}

	void UnBind()
	{
		mPixelBuffer.UnBind();
	}
	void Bind()
	{
		mPixelBuffer.Bind();
	}

	void PushData()
	{
		mPixelBuffer.PushData(mWidth, mHeight, mBytesPerPixel, mPixels);
	}
	uint8_t *PullData()
	{
		uint8_t *pix = mPixelBuffer.PullData(mWidth, mHeight, mBytesPerPixel);
		delete [] mPixels;
		mPixels = pix;
		return mPixels;
	}
	bool IsValid() const 
    { 
		return mPixelBuffer.IsValid();
	}

private:
	IPixelBufferHAL mPixelBuffer;
};

}
