
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


#include "renderer/GLES/GLESGPUProgram.h"
#include <gcl/Exception.h>
#include <gcl/Log.h>
#include <gcl/Matrix44.h>

#include "renderer/GLES/GLESShader.h"
#include "renderer/GLES/GLESShaderResource.h"
#include "renderer/GLES/GLESShaderResourceManager.h"
#include "renderer/GLES/GLESTexture.h"

#include "renderer/RenderPipe.h"


using namespace GCL;


GLESGPUProgram::GLESGPUProgram(const GLESShader &vshader, const GLESShader &pshader)
{
	mResource = GLESShaderResourceManager::Instance().LoadProgramResource(vshader.mGPUResource, pshader.mGPUResource);
}

GLESGPUProgram::~GLESGPUProgram()
{
	GLESShaderResourceManager::Instance().ReleaseProgramResource(mResource);
}

void GLESGPUProgram::Bind()
{
	RenderPipe::SendCommand([this](){
		GCLAssert(IsValidUnsafe());
	glUseProgram(mResource->GetProgramObjectUnsafe());glErrorCheck();
	});
}



void GLESGPUProgram::SetProjectionMatrix(const Matrix44 &m)
{
	RenderPipe::SendCommand([this, m](){
		GCLAssert(IsValidUnsafe());
	GLint projectionMatrixLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),"ProjectionMatrix");glErrorCheck();
	Matrix44f mf(m);
	glUniformMatrix4fv(projectionMatrixLoc,1,false,(const GLfloat*)&mf);glErrorCheck();
	});
}
void GLESGPUProgram::SetModelViewMatrix(const Matrix44 &m)
{
	RenderPipe::SendCommand([this, m](){
		GCLAssert(IsValidUnsafe());
	GLint modelviewMatrixLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),"ModelViewMatrix");glErrorCheck();
	Matrix44f mm(m);
	glUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);glErrorCheck();
	});
}

void GLESGPUProgram::SetUniform(const char *uniforName, long val)
{
	RenderPipe::SendCommand([this, uniforName, val](){
			GCLAssert(IsValidUnsafe());
	GLint uniformLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),uniforName);glErrorCheck();
	glUniform1i(uniformLoc,(GLint)val);glErrorCheck();
	});
}

void GLESGPUProgram::SetUniform(const char *uniforName, const Point2<long> &val)
{
	RenderPipe::SendCommand([this, uniforName, val](){
			GCLAssert(IsValidUnsafe());
	GLint uniformLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),uniforName);glErrorCheck();
	glUniform2i(uniformLoc, val.x, val.y);glErrorCheck();
	});
}

void GLESGPUProgram::SetUniform(const char *uniforName, const Point2<float> &val)
{
	RenderPipe::SendCommand([this, uniforName, val](){
			GCLAssert(IsValidUnsafe());
	GLint uniformLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),uniforName);glErrorCheck();
	glUniform2fv(uniformLoc,1, (GLfloat*)&val);glErrorCheck();
	});
}

void GLESGPUProgram::SetTextureSampler(const GLESTexture &sampler)
{
	RenderPipe::SendCommand([&](){
	GLint program = mResource->GetProgramObjectUnsafe();
	GLint textureLoc = glGetUniformLocation(program,"texture");glErrorCheck();
	glUniform1i(textureLoc, sampler.GetTextureUnitUnsafe());glErrorCheck();
	});
}

void GLESGPUProgram::GetUniform(const char *uniformName, Matrix44 &m44) const
{
	RenderPipe::SendCommandSync([&](){
			GCLAssert(IsValidUnsafe());
	GLint uniformLoc = glGetUniformLocation(mResource->GetProgramObjectUnsafe(),uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	GLfloat mf[16];
	glGetUniformfv(	mResource->GetProgramObjectUnsafe(),uniformLoc,mf);glErrorCheck();
	m44 = Matrix44((const float *)mf);
	});
}
void GLESGPUProgram::GetUniform(const char *uniformName, long &sampler) const
{
	RenderPipe::SendCommandSync([&](){
	GCLAssert(IsValidUnsafe());

	GLint program = mResource->GetProgramObjectUnsafe();
	GLint uniformLoc = glGetUniformLocation(program,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	GLint tempSampler;
	glGetUniformiv(program,uniformLoc,&tempSampler);glErrorCheck();
	sampler = tempSampler;
	});
}
long GLESGPUProgram::GetAttributeLocation(const char *attributeName) const
{
    long ret;
	RenderPipe::SendCommandSync([&](){
	ret = (long)glGetAttribLocation(mResource->GetProgramObjectUnsafe(),attributeName);glErrorCheck();
	});
	return ret;
}
void GLESGPUProgram::ResetDefault()
{
	RenderPipe::SendCommand([](){
	glUseProgram(0);glErrorCheck();
	});
}
