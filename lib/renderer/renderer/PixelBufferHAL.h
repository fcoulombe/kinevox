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
#include <3rdparty/OpenGL.h>
#include <gcl/PixelBuffer.h>

namespace GCL
{

class PixelBufferHAL : public PixelBuffer
{
public:
	PixelBufferHAL()
	: PixelBuffer(),
	  mBufferType(GL_STATIC_DRAW)
	{
	}
	PixelBufferHAL(const PixelBuffer &buffer)
	: mBufferType(GL_STATIC_DRAW)
	{
		mBitDepth = buffer.mBitDepth;
		mBitsPerPixel = buffer.mBitsPerPixel;
		mBytesPerPixel = buffer.mBytesPerPixel;
		mHeight = buffer.mHeight;
		mWidth = buffer.mWidth;
		mPixels = buffer.mPixels;
		glGenBuffersARB(1, &mPixelBufferId);glErrorCheck();
		/*Bind();
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mBytesPerPixel*mWidth*mHeight, 0, GL_STREAM_DRAW_ARB);glErrorCheck();
		UnBind();*/

	}

	template<typename PixelType>
	PixelBufferHAL(const PixelType *pixelArray, size_t width, size_t height)
	: PixelBuffer(pixelArray, width, height),
	  mBufferType(GL_STATIC_DRAW)
	  {
		glGenBuffersARB(1, &mPixelBufferId);glErrorCheck();
		Bind();
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, sizeof(PixelType)*width*height, 0, GL_STREAM_DRAW_ARB);glErrorCheck();
		UnBind();
	  }


	~PixelBufferHAL()
	{
		glDeleteBuffersARB(1, &mPixelBufferId);glErrorCheck();
	}

	void UnBind()
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);glErrorCheck();
	}
	void Bind()
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPixelBufferId);glErrorCheck();
	}

	void PushData()
	{
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_EXT, mBytesPerPixel*mWidth*mHeight, NULL, GL_STREAM_DRAW_ARB);glErrorCheck();
		uint8_t * pPixelsPBO = static_cast<uint8_t *>(glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_EXT, GL_WRITE_ONLY));glErrorCheck();
		// copy image data into the buffer
		memcpy(pPixelsPBO, mPixels, mBytesPerPixel*mWidth*mHeight);

		GLint res = glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_EXT);glErrorCheck();
		GCLAssertMsg(res == GL_TRUE, "Couldn't unmap pixel buffer. Exiting");
	}
	uint8_t *PullData()
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_EXT, mPixelBufferId);glErrorCheck();
		const uint8_t *pixels = static_cast<const uint8_t *>(glMapBufferARB(GL_PIXEL_PACK_BUFFER_EXT, GL_READ_ONLY_ARB));glErrorCheck();
		size_t bufferSize = mWidth*mHeight*mBytesPerPixel;
		uint8_t *buffer = new uint8_t[bufferSize];
		memcpy(buffer, pixels, bufferSize);
		//memcpy(mPixels, pixels, mWidth*mHeight*mBytesPerPixel);
		GLuint res = glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_EXT);glErrorCheck();
		GCLAssertMsg(res == GL_TRUE, "Couldn't unmap pixel buffer. Exiting");
		return buffer;
	}
	bool IsValid() const { return mPixelBufferId!=(GLuint)-1; }

private:
	GLuint mPixelBufferId;
	GLuint mBufferType;
};

}
