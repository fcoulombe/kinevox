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
#include "renderer/OpenGL.h"


namespace GCL
{
class RenderBuffer
{
public:
	RenderBuffer(size_t width, size_t height)
	: mRenderBufferId(-1)
	{
		glGenRenderbuffers(1, &mRenderBufferId); glErrorCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId);glErrorCheck();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);glErrorCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, 0); glErrorCheck();
	}
	~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &mRenderBufferId); glErrorCheck();
	}
	void Bind() { GCLAssert(IsValid()); glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId); glErrorCheck(); }

	bool IsValid() const { return (int)mRenderBufferId!=-1; }

	void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }

	static void ResetDefault() { GCLAssert(false && "TBD"); }

private:
	friend class FrameBuffer;
	GLuint GetRenderBufferId() const  { return  mRenderBufferId; }
	GLuint mRenderBufferId;
};

}
