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

#include <gcl/Assert.h>
#include "renderer/RenderCmd.h"
#include "renderer/RenderPipe.h"

namespace GCL
{

class Texture;
class RenderBuffer;
class FrameBuffer
{
public:
	FrameBuffer(const Texture & texture, const RenderBuffer & depthBuffer)
	{
		RenderPipe::SendCommand(RenderCommand3Arg(FRAMEBUFFER_CREATE, this, (void*)&texture, (void*)&depthBuffer));
	}
	~FrameBuffer()
	{
		RenderPipe::SendCommand(RenderCommand(FRAMEBUFFER_DESTROY, this));
	}
	void Bind()
	{
		RenderPipe::SendCommand(RenderCommand(FRAMEBUFFER_BIND, this));
	}
	static void ResetDefault()
	{

		RenderPipe::SendCommand(RenderCommand(FRAMEBUFFER_RESETDEFAULT));
	}

	bool IsValid() const { return RenderPipe::SendCommandSyncRet(RenderCommand(IS_FRAMEBUFFER_VALID, (void*)this)).GetBool(); }

	void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }
private:
};

}
