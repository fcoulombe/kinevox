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

#include "renderer/GLES/GLESShaderResource.h"
#include "renderer/ShaderResource.h"

#include <iostream>
#include <gcl/Assert.h>

using namespace GCL;

const GLenum GLESShaderResource::GLShaderType[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
#ifndef ES2
	GL_GEOMETRY_SHADER
#endif
};

const char *SHADER_HEADER =
	"#version 130 \n"
	"#pragma optimize(off) \n"
	"#pragma debug(on) \n"
	"\n";

GLESShaderResource::GLESShaderResource( const ShaderResource *shaderRes )
: GPUResource(),
mIsValid (false)
{
	RenderPipe::SendCommand([this, shaderRes](){
	mShaderObject = CompileShader(shaderRes);
	});
}

GLESShaderResource::~GLESShaderResource()
{
	RenderPipe::SendCommandSync([&](){
	if (mIsValid)
		glDeleteShader(mShaderObject);glErrorCheck();
	});
}


GLuint GLESShaderResource::CompileShader(const ShaderResource *shaderRes)
{
	GLenum type = GetShaderType((size_t)shaderRes->GetShaderType());
	const char *fileContent = shaderRes->GetShaderSource();

	std::string buffer(SHADER_HEADER);
	buffer += fileContent;


	GLuint shader=0;
	shader = glCreateShader(type); glErrorCheck();
	GCLAssert(shader != 0 && "glCreateShader(type);");
	const GLchar *cbuffer = (const GLchar *)buffer.c_str();
	glShaderSource(shader, 1, &cbuffer, NULL); glErrorCheck();
	glCompileShader(shader); glErrorCheck();
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); glErrorCheck();
	if(!compiled)
	{
		GLint infologLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);glErrorCheck();
		if (infologLength > 0)
		{
			GLchar *infoLog = new GLchar[infologLength];
			int charsWritten  = 0;
			glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);

			std::string log = "Shader Compile Log: " ;
			log += shaderRes->GetFilename();
			log += infoLog;

			delete [] infoLog;
			glDeleteShader(shader);glErrorCheck();
			GCLAssertMsg(false, log);
		}
		glDeleteShader(shader);glErrorCheck();
		return 0;
	}

	return shader;
}

void GLESShaderResource::PrintInfoLog(GLuint p)
{
	GLint infoLen = 0;
	glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);glErrorCheck();
	if(infoLen > 1)
	{
		char* infoLog = new char[(sizeof(char) * infoLen)];
		glGetProgramInfoLog(p, infoLen, NULL, infoLog);glErrorCheck();
		std::cerr << "Error linking program:\n%s" << infoLog << std::endl;
		delete [] infoLog;
	}
}



GLESGPUProgramResource::GLESGPUProgramResource( const GLESShaderResource *pshader, const GLESShaderResource *vshader )
	: mRefCount(1),mIsValid (false)
{

	RenderPipe::SendCommand([this, pshader, vshader](){
	mProgramObject = glCreateProgram();glErrorCheck();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");
	AttachShader(*pshader);
	AttachShader(*vshader);
	Link();
	});
	
}

GCL::GLESGPUProgramResource::~GLESGPUProgramResource()
{
	RenderPipe::SendCommandSync([&](){
	if (mIsValid)
		glDeleteProgram(mProgramObject);glErrorCheck();
	});
}

void GLESGPUProgramResource::AttachShader(const GLESShaderResource &shader)
{
	GLuint shaderObject = shader.GetShaderObjectUnsafe();
	glAttachShader(mProgramObject, shaderObject);glErrorCheck();
}

void GLESGPUProgramResource::Link()
{
	glLinkProgram(mProgramObject);glErrorCheck();

	GLint linked;
	glGetProgramiv(mProgramObject, GL_LINK_STATUS, &linked);glErrorCheck();
	if(linked)
	{
		mIsValid = true;
	}
	else
	{
		PrintInfoLog(mProgramObject);
		glDeleteProgram(mProgramObject);glErrorCheck();
		mIsValid = false;
	}
}


void GLESGPUProgramResource::PrintInfoLog(GLuint p)
{
	GLint infoLen = 0;
	glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);glErrorCheck();
	if(infoLen > 1)
	{
		char* infoLog = new char[(sizeof(char) * infoLen)];
		glGetProgramInfoLog(p, infoLen, NULL, infoLog);glErrorCheck();
		std::cerr << "Error linking program:\n%s" << infoLog << std::endl;
		delete [] infoLog;
	}
}
