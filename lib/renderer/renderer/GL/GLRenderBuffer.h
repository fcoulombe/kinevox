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
class GLRenderBuffer
{
public:
	GLRenderBuffer(size_t width, size_t height)
	: mRenderBufferId((GLuint)-1)
	{
		RenderPipe::SendCommand([this, width, height](){
		glGenRenderbuffers(1, &mRenderBufferId); glErrorCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId);glErrorCheck();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
							 (GLsizei)width, (GLsizei)height);glErrorCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, 0); glErrorCheck();
		});
	}
	~GLRenderBuffer()
	{
		RenderPipe::SendCommandSync([&](){
		glDeleteRenderbuffers(1, &mRenderBufferId); glErrorCheck();
		});
	}
	void Bind() 
	{ 
		RenderPipe::SendCommand([&](){
			GCLAssert(IsValidUnsafe()); 
			glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId); glErrorCheck(); 
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
private:
	bool IsValidUnsafe() const 
	{
		bool ret;
		ret = (int)mRenderBufferId!=-1; 
		return ret;
	}
	friend class GLFrameBuffer;
	GLuint GetRenderBufferId() const  { return  mRenderBufferId; }
	GLuint mRenderBufferId;
};

}
