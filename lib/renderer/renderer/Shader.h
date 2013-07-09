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

#include "renderer/RenderCmd.h"
#include "renderer/RenderPipe.h"
namespace GCL
{

enum ShaderType
{
	VERTEX_SHADER=0,
	FRAGMENT_SHADER=1,
	GEOMETRY_SHADER=2
};

  class Shader
  {
  public:
    Shader(const char *shaderSourcePath, ShaderType type)
	{
		size_t fileNameLen = strlen(shaderSourcePath)+1;
		char *xferbuffer = new char[fileNameLen];
		strncpy(xferbuffer, shaderSourcePath, fileNameLen);
		RenderPipe::SendCommand(new RenderCommand3Arg(SHADER_CREATE, this, (void*)xferbuffer, (void*)type));
	}
    ~Shader() 
	{
		RenderPipe::SendCommand(new RenderCommand(SHADER_DESTROY, this));
	}
    bool IsValid() const { return RenderPipe::SendCommandSyncRet(new RenderCommand(IS_SHADER_VALID, (void*)this)).GetBool(); }

private:
	
  };
}
