/*
 * Copyright (C) 2014 by Francois Coulombe
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
#include "renderer/GPUResource.h"

namespace GCL
{
	class ShaderResource;
	class GLShaderResource : public GPUResource
	{
	public:
		GLShaderResource(const ShaderResource *resource);
		~GLShaderResource();
		bool IsValid() const { return mIsValid; }
		static GLenum GetShaderType(size_t type) { return GLShaderType[type]; }
		GLuint GetShaderObject() const { return mShaderObject; }
	private:
		GLShaderResource() {}
		GLuint CompileShader(const ShaderResource *shaderRes);
		void PrintInfoLog(GLuint );
		GLuint mShaderObject;
		bool mIsValid;
		static const GLenum GLShaderType[];
	};

	class GLGPUProgramResource
	{
	public:
		GLGPUProgramResource(const GLShaderResource *pshader, const GLShaderResource *vshader );
		~GLGPUProgramResource();
		bool IsValid() const { return mIsValid; }
		GLuint GetProgramObject() const { return mProgramObject; }
		size_t mRefCount;
	private:
		void AttachShader(const GLShaderResource &shader);
		void Link();
		void PrintInfoLog(GLuint );
		GLuint mProgramObject;
		bool mIsValid;
	};
}
