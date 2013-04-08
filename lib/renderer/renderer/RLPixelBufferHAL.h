/*
 * Copyright (C) 2013 by Francois Coulombe
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
#include <3rdparty/OpenGL.h>
#include <gcl/PixelBuffer.h>

namespace GCL
{

class RLPixelBufferHAL : public PixelBuffer
{
public:
	RLPixelBufferHAL()
	: PixelBuffer(),
	  mBufferType(RL_STATIC_DRAW)
	{
        rlGenBuffers(1, &mPixelBufferId);
	}
	RLPixelBufferHAL(const PixelBuffer &buffer)
	: mBufferType(RL_STATIC_DRAW)
	{
		mBitDepth = buffer.mBitDepth;
		mBitsPerPixel = buffer.mBitsPerPixel;
		mBytesPerPixel = buffer.mBytesPerPixel;
		mHeight = buffer.mHeight;
		mWidth = buffer.mWidth;
		mPixels = buffer.mPixels;
		rlGenBuffers(1, &mPixelBufferId);
	}

	template<typename PixelType>
	RLPixelBufferHAL(const PixelType *pixelArray, size_t width, size_t height)
	: PixelBuffer(pixelArray, width, height),
	  mBufferType(RL_STATIC_DRAW)
	  {
		rlGenBuffers(1, &mPixelBufferId);
		Bind();
		rlBufferData(RL_PIXEL_UNPACK_BUFFER, sizeof(PixelType)*width*height, 0, RL_STREAM_DRAW);
		UnBind();
	  }


	~RLPixelBufferHAL()
	{
		rlDeleteBuffers(1, &mPixelBufferId);
	}

	void UnBindPack()
	{
		rlBindBuffer(RL_PIXEL_PACK_BUFFER, 0);
	}
    void UnBindUnPack()
    {
        rlBindBuffer(RL_PIXEL_UNPACK_BUFFER, 0);
    }
	void BindPack()
	{
		rlBindBuffer(RL_PIXEL_PACK_BUFFER, mPixelBufferId);
	}
    void BindUnPack()
    {
        rlBindBuffer(RL_PIXEL_UNPACK_BUFFER, mPixelBufferId);
    }

	void PushData()
	{
        //create at least create the buffer in case we need to use it to pull right away
		rlBufferData(RL_PIXEL_UNPACK_BUFFER, mBytesPerPixel*mWidth*mHeight, NULL, RL_STREAM_DRAW);
        if (mPixels)
        {
            uint8_t * pPixelsPBO = static_cast<uint8_t *>(rlMapBuffer(RL_PIXEL_UNPACK_BUFFER, RL_WRITE_ONLY));
            // copy image data into the buffer
            memcpy(pPixelsPBO, mPixels, mBytesPerPixel*mWidth*mHeight);

            RLint res = rlUnmapBuffer(RL_PIXEL_UNPACK_BUFFER);
            GCLAssertMsg(res == RL_TRUE, "Couldn't unmap pixel buffer. Exiting");
        }
	}
	uint8_t *PullData()
	{
		const uint8_t *pixels = static_cast<const uint8_t *>(rlMapBuffer(RL_PIXEL_PACK_BUFFER, RL_READ_ONLY));
		size_t bufferSize = mWidth*mHeight*mBytesPerPixel;
		uint8_t *buffer = new uint8_t[bufferSize];
		memcpy(buffer, pixels, bufferSize);
		RLuint res = rlUnmapBuffer(RL_PIXEL_PACK_BUFFER);
		GCLAssertMsg(res == RL_TRUE, "Couldn't unmap pixel buffer. Exiting");
		return buffer;
	}
	bool IsValid() const { return mPixelBufferId!=(RLbuffer)-1; }

private:
	RLbuffer mPixelBufferId;
	RLuint mBufferType;
};

}
