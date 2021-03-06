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
#include "renderer/RenderPipe.h"
#include <cstring>

namespace GCL
{

class GLPixelBufferHAL
{
public:
	GLPixelBufferHAL()
	: mBufferType(GL_STREAM_DRAW)
	{
		RenderPipe::SendCommand([&](){
        glGenBuffers(1, &mPixelBufferId);glErrorCheck();
		});
	}


	~GLPixelBufferHAL()
	{
		RenderPipe::SendCommandSync([&](){
		glDeleteBuffers(1, &mPixelBufferId);glErrorCheck();
		});
	}

	void UnBind()
	{
		RenderPipe::SendCommand([&](){
		UnBindUnsafe();
		});
	}
	void Bind()
	{
		RenderPipe::SendCommand([&](){
		BindUnsafe();
		});
	}

	void PushData(size_t width, size_t height, size_t bytesPerPixel, const uint8_t *pixels)
	{
		RenderPipe::SendCommand([=](){
        PushDataUnsafe(width, height, bytesPerPixel, pixels);
		});
	}
	uint8_t *PullData(size_t width, size_t height, size_t bytesPerPixel)
	{
		uint8_t *buffer;
		RenderPipe::SendCommandSync([&](){
		buffer = PullDataUnsafe(width, height, bytesPerPixel);
		});
		return buffer;

	}
	bool IsValid() const 
    { 
		bool ret;
		RenderPipe::SendCommandSync([&](){
        ret= IsValidUnsafe(); 
		});
		return ret;
    }

private:
	friend class GLTextureResource;
	bool IsValidUnsafe() const 
	{ 
		bool ret;
		ret= mPixelBufferId!=(GLuint)-1; 
		return ret;
	}
	GLuint mPixelBufferId;
	GLuint mBufferType;
	void UnBindUnsafe()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);glErrorCheck();
	}
	void BindUnsafe()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPixelBufferId);glErrorCheck();
	}
	void PushDataUnsafe(size_t width, size_t height, size_t bytesPerPixel, const uint8_t *pixels)
	{
		glBufferData(GL_PIXEL_UNPACK_BUFFER, bytesPerPixel*width*height, NULL, mBufferType);glErrorCheck();
		if (pixels)
		{
			uint8_t * pPixelsPBO = static_cast<uint8_t *>(glMapBufferARB(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY));glErrorCheck();
			// copy image data into the buffer
			memcpy(pPixelsPBO, pixels, bytesPerPixel*width*height);

			GLint res = glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);glErrorCheck();
			GCLAssertMsg(res == GL_TRUE, "Couldn't unmap pixel buffer. Exiting");
		}
	}
	uint8_t *PullDataUnsafe(size_t width, size_t height, size_t bytesPerPixel)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, mPixelBufferId);glErrorCheck();
		const uint8_t *pixels = static_cast<const uint8_t *>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));glErrorCheck();
		size_t bufferSize = width*height*bytesPerPixel;
		uint8_t *buffer = new uint8_t[bufferSize];
		memcpy(buffer, pixels, bufferSize);
		//memcpy(mPixels, pixels, mWidth*mHeight*mBytesPerPixel);
		GLuint res = glUnmapBuffer(GL_PIXEL_PACK_BUFFER);glErrorCheck();
		GCLAssertMsg(res == GL_TRUE, "Couldn't unmap pixel buffer. Exiting");
		return buffer;
	}
};

}
