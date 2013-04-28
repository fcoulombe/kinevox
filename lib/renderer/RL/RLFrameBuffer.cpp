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

#if USE_OPENRL
#include "renderer/RLFrameBuffer.h"

#include <sstream>

//#include "renderer/RenderBuffer.h"
#include "renderer/RLTexture.h"

using namespace GCL;

namespace
{
void checkFrameBufferStatus(RLenum stat)
{
	if (RL_FRAMEBUFFER_COMPLETE == stat)
		return;

	std::stringstream s;
	switch (stat)
	{
	case RL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		s << "RL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS is returned if the z-offset exceeds any attached texture depth or the frame buffer width and height are not within all attached texture width and height values.";
		break;
	case RL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
		s << "RL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete.";
		break;
	case RL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		s<< "RL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT is returned if the framebuffer does not have at least one image attached to it.";
		break;
	case RL_FRAMEBUFFER_INCOMPLETE_FORMATS :
		s << "RL_FRAMEBUFFER_INCOMPLETE_FORMATS is returned if any attached texture has an unsupported format.";
		break;
	default:
		s<<"don't know what happenned to the framebuffer";
	}
    
	GCLAssertMsg(RL_FRAMEBUFFER_COMPLETE == stat, s.str().c_str());
}
}

RLFrameBuffer::RLFrameBuffer(const RLTexture & texture/*, const RenderBuffer & depthBuffer*/)
: mFrameBufferId((RLframebuffer)-1)
{
	GCLAssert(texture.IsValid());

	rlGenFramebuffers(1, &mFrameBufferId); 
	rlBindFramebuffer(RL_FRAMEBUFFER, mFrameBufferId); 
	rlFramebufferTexture2D(RL_FRAMEBUFFER, RL_COLOR_ATTACHMENT0, RL_TEXTURE_2D, texture.GetTextureId(), 0); 
	//rlFramebufferRenderbuffer(RL_FRAMEBUFFER, RL_DEPTH_ATTACHMENT, RL_RENDERBUFFER, depthBuffer.GetRenderBufferId());

	RLenum status =  rlCheckFramebufferStatus(RL_FRAMEBUFFER);
	checkFrameBufferStatus(status);

	rlBindFramebuffer(RL_FRAMEBUFFER, 0);
}
#endif
