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

#include "rendererconf.h"
#include GFXAPI_PixelBufferHAL_H

#include <gcl/PixelBuffer.h>

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
	{
		mBitDepth = buffer.mBitDepth;
		mBitsPerPixel = buffer.mBitsPerPixel;
		mBytesPerPixel = buffer.mBytesPerPixel;
		mHeight = buffer.mHeight;
		mWidth = buffer.mWidth;
		mPixels = buffer.mPixels;
	}

	template<typename PixelType>
	PixelBufferHAL(const PixelType *pixelArray, size_t width, size_t height)
	: PixelBuffer(pixelArray, width, height)
	  {
		Bind();
		mPimpl.PushData(width, height, PixelType::OffsetToNext(), 0);
		UnBind();
	  }


	~PixelBufferHAL()
	{
	}

	void UnBind()
	{
		mPimpl.UnBind();
	}
	void Bind()
	{
		mPimpl.Bind();
	}

	void PushData()
	{
		mPimpl.PushData(mWidth, mHeight, mBytesPerPixel, mPixels);
	}
	uint8_t *PullData()
	{
		return mPimpl.PullData(mWidth, mHeight, mBytesPerPixel);
	}
	bool IsValid() const 
    { 
		return mPimpl.IsValid();
    }

private:
	IPixelBufferHAL mPimpl;

};

}
