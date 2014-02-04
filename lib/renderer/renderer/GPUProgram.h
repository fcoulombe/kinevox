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

//#include "renderer/RenderCmd.h"
//#include "renderer/RenderPipe.h"
#include "renderer/Shader.h"
#include "renderer/ShaderAttributeLocations.h"
#include "renderer/Texture.h"
#include "rendererconf.h"
#include GFXAPI_GPUProgram_H
namespace GCL
{
class Matrix44;
class Texture;
class Shader;
  class GPUProgram
  {
  public:
	GPUProgram(Shader &vshader, Shader &pshader) 
		: mProgram(*pshader.mShader, *vshader.mShader)
	{ 
	}

    ~GPUProgram() 
	{
		mShaderList.clear();
	}
    void Bind() 
	{
		mProgram.Bind();
	}
	bool IsValid() const {  return mProgram.IsValid(); }

	AttribLocations GetShaderLocations() const
	{
		AttribLocations loc;
		loc.position = GetAttributeLocation("InPosition");
		loc.normal = GetAttributeLocation("InNormal");
		loc.texCoord = GetAttributeLocation("InTexCoord");
		return loc;
	}
	void SetTextureSampler(const Texture &sampler) { mProgram.SetTextureSampler(*sampler.mTexture); } 
	void SetProjectionMatrix(const Matrix44 &projection) 
	{ 
		mProgram.SetProjectionMatrix(projection);
	}
	void SetModelViewMatrix(const Matrix44 &m) { mProgram.SetModelViewMatrix(m); }
	void SetUniform(const char *uniformName, long value) { mProgram.SetUniform(uniformName, value); }
	void SetUniform(const char *uniformName, const Point2<float> &value) { mProgram.SetUniform(uniformName, value); }
	void SetUniform(const char *uniformName, const Point2<long> &value) { mProgram.SetUniform(uniformName, value); }

	void GetUniform(const char *unformName, Matrix44 &m44) const {  mProgram.GetUniform(unformName, m44); }
	void GetUniform(const char *unformName, long &ret) const { mProgram.GetUniform(unformName, ret); }
	int GetAttributeLocation(const char *attributeName) const { return mProgram.GetAttributeLocation(attributeName); }

	static void ResetDefault() { IGPUProgram::ResetDefault(); }
  private:
	std::vector<Shader *> mShaderList;
	IGPUProgram mProgram;
  };
}
