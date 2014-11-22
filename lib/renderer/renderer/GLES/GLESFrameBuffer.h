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

namespace GCL
{
class GLESTexture;
class GLESRenderBuffer;
class GLESFrameBuffer
{
public:
	GLESFrameBuffer(const GLESTexture &texture, const GLESRenderBuffer &depthBuffer);
	~GLESFrameBuffer()
	{
		RenderPipe::SendCommandSync([this](){
		glDeleteFramebuffers(1, &mFrameBufferId); glErrorCheck();
		});
	}
	void Bind()
	{
		RenderPipe::SendCommand([this](){
		GCLAssert(IsValidUnsafe()); glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);  glErrorCheck();
		});
	}


	bool IsValid() const
	{
		bool ret;
		RenderPipe::SendCommandSync([&](){
		ret = IsValidUnsafe();
		});
		return ret;
	}

	static void ResetDefault()
	{
		RenderPipe::SendCommand([&](){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  glErrorCheck();
		});
	}

private:
	GLuint mFrameBufferId;
	bool IsValidUnsafe() const
	{
		bool ret;
		ret = (int)mFrameBufferId!=-1;
		return ret;
	}
};

}
