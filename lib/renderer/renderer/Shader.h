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

#include "rendererconf.h"
#include GFXAPI_Shader_H
#include "renderer/ShaderResource.h"
#include "renderer/ShaderResourceManager.h"
namespace GCL
{
  class Shader
  {
  public:
    Shader(const char *shaderSourcePath, ShaderType type)
	{
#if USE_OPENGL3
		const std::string shaderFileName = std::string(GLSL_PATH) + std::string(shaderSourcePath)+std::string(".glsl3");
#elif defined(ES2)
		const std::string shaderFileName = std::string(GLSL_PATH) + std::string(shaderSourcePath)+std::string(".glsles");

#else
		const std::string shaderFileName = std::string(GLSL_PATH) + std::string(shaderSourcePath)+std::string(".glsl");
#endif
		const Resource *tempResource = ShaderResourceManager::Instance().LoadResource(shaderFileName.c_str());
		mResource = static_cast<const ShaderResource*>(tempResource);
		mResource->SetShaderType(type);
		mShader = new IShader(mResource);
	}
    ~Shader() 
	{
		delete mShader;
		ShaderResourceManager::Instance().ReleaseResource(mResource);
	}
	bool IsValid() const { return mShader->IsValid(); }
private:
	const ShaderResource *mResource;
	IShader *mShader;
	friend class GPUProgram;
  };
}
