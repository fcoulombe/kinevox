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
#include <vector>

#include "renderer/RenderCmd.h"
#include "renderer/RenderPipe.h"
#include "renderer/ShaderAttributeLocations.h"
namespace GCL
{
class Matrix44;
class Texture;
class Shader;
  class GPUProgram
  {
  public:
	GPUProgram() 
	{ 
		RenderPipe::SendCommand(new RenderCommand(GPUPROGRAM_CREATE, this)); 
	}

    ~GPUProgram() 
	{
		RenderPipe::SendCommand(new RenderCommand(GPUPROGRAM_DESTROY, this)); 

		mShaderList.clear();
	}
    void Bind() 
	{
		RenderPipe::SendCommand(new RenderCommand(GPUPROGRAM_BIND, this)); 
	}
    void AttachShader(Shader &shader)
    {
    	mShaderList.push_back(&shader);
		RenderPipe::SendCommand(new RenderCommand2Arg(GPUPROGRAM_ATTACH_SHADER, this, &shader)); 
    }
	void Link() { RenderPipe::SendCommand(new RenderCommand(GPUPROGRAM_LINK, this));  }
    bool IsValid() const {  return RenderPipe::SendCommandSyncRet(new RenderCommand(IS_GPUPROGRAM_VALID, (void*)this)).GetBool(); }

	AttribLocations GetShaderLocations() const
	{
		AttribLocations loc;
		loc.position = GetAttributeLocation("InPosition");
		loc.normal = GetAttributeLocation("InNormal");
		loc.texCoord = GetAttributeLocation("InTexCoord");
		return loc;
	}
    void SetTextureSampler(const Texture &sampler) { RenderPipe::SendCommand(new RenderCommand2Arg(GPUPROGRAM_SET_TEXTURE_SAMPLER, (void*)this, (void*)&sampler)); }
    void SetProjectionMatrix(const Matrix44 &m) 
	{ 
		RenderPipe::SendCommand(new RenderCommandMatArg(GPUPROGRAM_SET_PROJECTION, (void*)this, m));  
	}
    void SetModelViewMatrix(const Matrix44 &m) { RenderPipe::SendCommand(new RenderCommandMatArg(GPUPROGRAM_SET_MODELVIEW, (void*)this, m));}
    void GetUniform(const char *unformName, Matrix44 &m44) const { m44 = RenderPipe::SendCommandSyncRet(new RenderCommand2Arg(GPUPROGRAM_GET_UNIFORM_MATRIX, (void*)this, (void*)unformName)).GetMatrix(); }
    void GetUniform(const char *unformName, int &ret) const { ret = RenderPipe::SendCommandSyncRet(new RenderCommand2Arg(GPUPROGRAM_GET_UNIFORM_NUMBER, (void*)this, (void*)unformName)).GetNumber();  }
    int GetAttributeLocation(const char *attributeName) const { return RenderPipe::SendCommandSyncRet(new RenderCommand2Arg(GPUPROGRAM_GET_ATTRIBUTE_LOCATION, (void*)this, (void*)attributeName)).GetNumber();   }

    static void ResetDefault() { RenderPipe::SendCommand(new RenderCommand(GPUPROGRAM_RESETDEFAULT)); }
  private:
	std::vector<Shader *> mShaderList;
  };
}
