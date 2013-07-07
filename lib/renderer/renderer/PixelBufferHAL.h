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
#include "renderer/RenderCmd.h"
#include "renderer/RenderPipe.h"
namespace GCL
{

class PixelBufferHAL : public PixelBuffer
{
public:
	PixelBufferHAL()
	: PixelBuffer()
	{
		RenderPipe::SendCommand(new RenderCommand(PBO_CREATE, this));
	}
	PixelBufferHAL(const PixelBuffer &buffer)
	:	PixelBuffer(buffer)
	{
		RenderPipe::SendCommand(new RenderCommand(PBO_CREATE, this));
	}

#if 1
	template<typename PixelType>
	PixelBufferHAL(const PixelType *pixelArray, size_t width, size_t height)
	: PixelBuffer(pixelArray, width, height)
	  {
		RenderPipe::SendCommand(new RenderCommand(PBO_CREATE, this));
		Bind();
		RenderPipe::SendCommand(new RenderCommand5Arg(PBO_PUSH, this, (void*)width, (void*)height, (void*)PixelType::OffsetToNext(), 0));
		UnBind();
	  }
#endif

	~PixelBufferHAL()
	{
		RenderPipe::SendCommand(new RenderCommand(PBO_DESTROY, this));
	}

	void UnBind()
	{
		RenderPipe::SendCommand(new RenderCommand(PBO_UNBIND, this));
	}
	void Bind()
	{
		RenderPipe::SendCommand(new RenderCommand(PBO_BIND, this));
	}

	void PushData()
	{
		uint8_t *xferBuffer = new uint8_t[mWidth * mHeight*mBytesPerPixel];
		memcpy(xferBuffer, mPixels, mWidth * mHeight*mBytesPerPixel);
		RenderPipe::SendCommand(new RenderCommand5Arg(PBO_PUSH, this, (void*)mWidth, (void*)mHeight, (void*)mBytesPerPixel, xferBuffer));
	}
	uint8_t *PullData()
	{
		 return (uint8_t *)RenderPipe::SendCommandSyncRet(new RenderCommand4Arg(PBO_PULL, (void*)this, (void*)mWidth, (void*)mHeight, (void*)mBytesPerPixel)).GetPointer();
	}
	bool IsValid() const 
    { 
		 return RenderPipe::SendCommandSyncRet(new RenderCommand(IS_PBO_VALID, (void*)this)).GetBool();
    }

private:
};

}
