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
#if USE_OPENRL
#include "renderer/RLProgram.h"
#include "renderer/RLShader.h"
#include <gcl/Exception.h>
#include <gcl/File.h>
#include <gcl/Matrix44.h>
#include <gcl/ResourceManagerConfig.h>

#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/RLTexture.h"

using namespace GCL;


RLProgram::RLProgram()
:mIsValid (false)
{
	mProgramObject = rlCreateProgram();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");

}

void RLProgram::AttachShader(RLShader &shader)
{
    mShaderList.push_back(&shader);
    rlAttachShader(mProgramObject, shader.mShader);
}
void RLProgram::Link()
{
    rlLinkProgram(mProgramObject);

    RLint linked;
    rlGetProgramiv(mProgramObject, RL_LINK_STATUS, &linked);
    if(linked != RL_FALSE)
    {
        mIsValid = true;
    }
    else
    {
        PrintInfoLog(mProgramObject);
        rlDeleteProgram(mProgramObject);
        mIsValid = false;
    }
}

RLProgram::~RLProgram()
{
    if (mIsValid)
        rlDeleteProgram(mProgramObject);
}

void RLProgram::Bind()
{
	GCLAssert(mIsValid);
	rlUseProgram(mProgramObject);
}


void RLProgram::PrintInfoLog(RLprogram p)
{
    const char* infoLog;
    rlGetProgramString(p, RL_LINK_LOG, &infoLog);
    std::string linkLog =  std::string("Error linking program:\n") + infoLog ;
    throw GCLException(linkLog);
}


void RLProgram::SetProjectionMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	RLint projectionMatrixLoc = rlGetUniformLocation(mProgramObject,"ProjectionMatrix");
	Matrix44f mf(m);
	rlUniformMatrix4fv(projectionMatrixLoc,1,false,(const RLfloat*)&mf);
}
void RLProgram::SetModelViewMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	RLint modelviewMatrixLoc = rlGetUniformLocation(mProgramObject,"ModelViewMatrix");
	Matrix44f mm(m);
	rlUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);
}

void RLProgram::SetTextureSampler(const RLTexture &sampler)
{
	RLint textureLoc = rlGetUniformLocation(mProgramObject,"texture");
	rlUniform1i(textureLoc, sampler.GetTextureUnit());
}

void RLProgram::GetUniform(const char *uniformName, Matrix44 &m44) const
{
	RLfloat mf[16];
	GCLAssert(mIsValid);
	RLint uniformLoc = rlGetUniformLocation(mProgramObject,uniformName);
	GCLAssert(uniformLoc!=-1);
	rlGetUniformfv(	mProgramObject,uniformLoc,mf);
	m44 = Matrix44((const float *)mf);
}
void RLProgram::GetUniform(const char *uniformName, int &sampler) const
{
	GCLAssert(mIsValid);
	RLint uniformLoc = rlGetUniformLocation(mProgramObject,uniformName);
	GCLAssert(uniformLoc!=-1);
	rlGetUniformiv(mProgramObject,uniformLoc,&sampler);
}
int RLProgram::GetAttributeLocation(const char *attributeName) const
{
    int ret=-1;

	ret =  (int)rlGetAttribLocation(mProgramObject,attributeName);
	GCLAssertMsg(ret!=-1, attributeName);

	return ret;
}
void RLProgram::ResetDefault()
{
	rlUseProgram(0);
}
#endif
