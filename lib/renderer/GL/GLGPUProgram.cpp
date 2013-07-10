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

#include "renderer/GL/GLGPUProgram.h"
#include <gcl/Exception.h>
#include <gcl/Matrix44.h>

#include "renderer/GL/GLShader.h"
#include "renderer/GL/GLTexture.h"

using namespace GCL;

GLGPUProgram::GLGPUProgram()
:mIsValid (false)
{
	mProgramObject = glCreateProgram();glErrorCheck();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");
	return;
}

GLGPUProgram::~GLGPUProgram()
{
    if (mIsValid)
        glDeleteProgram(mProgramObject);glErrorCheck();
}

void GLGPUProgram::Bind()
{
	GCLAssert(mIsValid);
	glUseProgram(mProgramObject);glErrorCheck();
}

void GLGPUProgram::AttachShader(const GLShader &shader)
{
    glAttachShader(mProgramObject, shader.mShaderObject);glErrorCheck();
}

void GLGPUProgram::Link()
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


void GLGPUProgram::PrintInfoLog(GLuint p)
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


void GLGPUProgram::SetProjectionMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	GLint projectionMatrixLoc = glGetUniformLocation(mProgramObject,"ProjectionMatrix");glErrorCheck();
	Matrix44f mf(m);
	glUniformMatrix4fv(projectionMatrixLoc,1,false,(const GLfloat*)&mf);glErrorCheck();
}
void GLGPUProgram::SetModelViewMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	GLint modelviewMatrixLoc = glGetUniformLocation(mProgramObject,"ModelViewMatrix");glErrorCheck();
	Matrix44f mm(m);
	glUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);glErrorCheck();
}

void GLGPUProgram::SetUniform(const char *uniforName, int val)
{
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniforName);glErrorCheck();
	glUniform1i(uniformLoc,(GLint)val);glErrorCheck();
}

void GLGPUProgram::SetUniform(const char *uniforName, const Point2<int> &val)
{
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniforName);glErrorCheck();
	glUniform2iv(uniformLoc,1, (GLint*)&val);glErrorCheck();
}

void GLGPUProgram::SetUniform(const char *uniforName, const Point2<float> &val)
{
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniforName);glErrorCheck();
	glUniform2fv(uniformLoc,1, (GLfloat*)&val);glErrorCheck();
}

void GLGPUProgram::SetTextureSampler(const GLTexture &sampler)
{
	GLint textureLoc = glGetUniformLocation(mProgramObject,"texture");glErrorCheck();
	glUniform1i(textureLoc, sampler.GetTextureUnit());glErrorCheck();
}

void GLGPUProgram::GetUniform(const char *uniformName, Matrix44 &m44) const
{
	GLfloat mf[16];
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformfv(	mProgramObject,uniformLoc,mf);glErrorCheck();
	m44 = Matrix44((const float *)mf);
}
void GLGPUProgram::GetUniform(const char *uniformName, int &sampler) const
{
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformiv(mProgramObject,uniformLoc,&sampler);glErrorCheck();
}
int GLGPUProgram::GetAttributeLocation(const char *attributeName) const
{
    int ret=-1;
	ret =  (int)glGetAttribLocation(mProgramObject,attributeName);glErrorCheck();
	//GCLAssertMsg(ret!=-1, attributeName);
	return ret;
}
void GLGPUProgram::ResetDefault()
{
	glUseProgram(0);glErrorCheck();
}
