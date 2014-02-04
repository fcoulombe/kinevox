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
#include "renderer/RenderPipe.h"

namespace GCL
{
	class ShaderResource;
	class GLShaderResource : public GPUResource
	{
	public:
		GLShaderResource(const ShaderResource *resource);
		~GLShaderResource();
		bool IsValid() const 
		{
			bool ret;
			RenderPipe::SendCommandSync([&](){
			ret = IsValidUnsafe(); 
			});
			return ret;
		}
		static GLenum GetShaderType(size_t type) { return GLShaderType[type]; }
		GLuint GetShaderObject() const 
		{ 
			GLuint ret;
			RenderPipe::SendCommandSync([&](){
				ret = GetShaderObjectUnsafe(); 
			});
			return ret;
		}
	private:
		friend class GLGPUProgramResource;
		GLShaderResource() {}
		GLuint CompileShader(const ShaderResource *shaderRes);
		void PrintInfoLog(GLuint );
		GLuint mShaderObject;
		bool mIsValid;
		static const GLenum GLShaderType[];

		bool IsValidUnsafe() const { return mIsValid; }
		GLuint GetShaderObjectUnsafe() const { return mShaderObject; }
	};

	class GLGPUProgramResource
	{
	public:
		GLGPUProgramResource(const GLShaderResource *pshader, const GLShaderResource *vshader );
		~GLGPUProgramResource();
		bool IsValid() const
		{ 
			bool ret;
			RenderPipe::SendCommandSync([&](){
				ret = IsValidUnsafe(); 
			});
			return ret;
		}
		GLuint GetProgramObject() const 
		{ 
			GLuint ret;
			RenderPipe::SendCommandSync([&](){
				ret = GetProgramObjectUnsafe(); 
			});
			return ret;
		}
		size_t mRefCount;
	private:
		friend class GLGPUProgram;
		void AttachShader(const GLShaderResource &shader); //unsafe
		void Link(); //unsafe
		void PrintInfoLog(GLuint ); //unsafe
		GLuint mProgramObject;
		bool mIsValid;

		bool IsValidUnsafe() const { return mIsValid; }
		GLuint GetProgramObjectUnsafe() const { return mProgramObject; }
	};
}
