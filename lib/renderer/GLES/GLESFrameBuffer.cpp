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

#include "renderer/GLES/GLESFrameBuffer.h"

#include <sstream>

#include "renderer/GLES/GLESRenderBuffer.h"
#include "renderer/GLES/GLESTexture.h"

using namespace GCL;

namespace
{
void checkFrameBufferStatus(GLenum stat)
{
	if (GL_FRAMEBUFFER_COMPLETE == stat)
		return;
	GCLAssert(GL_FRAMEBUFFER_COMPLETE == stat);
}
}

GLESFrameBuffer::GLESFrameBuffer(const GLESTexture & texture, const GLESRenderBuffer & depthBuffer)
: mFrameBufferId((GLuint)-1)
{
	GCLAssert(texture.IsValid());
	RenderPipe::SendCommand([&](){

	glGenFramebuffers(1, &mFrameBufferId); glErrorCheck();
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId); glErrorCheck();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureIdUnsafe(), 0); glErrorCheck();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.GetRenderBufferId());glErrorCheck();

	GLenum status =  glCheckFramebufferStatus(GL_FRAMEBUFFER);
	checkFrameBufferStatus(status);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);glErrorCheck();
	});
}
