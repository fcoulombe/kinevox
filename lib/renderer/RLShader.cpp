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

#include "renderer/RLShader.h"
#include <gcl/Exception.h>
#include <gcl/File.h>
#include <gcl/Matrix44.h>
#include <gcl/ResourceManagerConfig.h>

#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/RLTexture.h"

using namespace GCL;

namespace 
{
    char *LoadShader(const char *filename)
    {
        GCLAssertMsg(GCLFile::Exists(filename), filename);

        GCLFile fp(filename);
        size_t fileSize = fp.GetFileSize();
        char *fileContent = new char[fileSize+1];
        fp.Read(fileContent, fileSize);
        fileContent[fileSize] = 0;
        return fileContent;
    }
}
RLShader::RLShader(const char *frameShader , const char *vertexShader , const char *rayShader )
:mIsValid (false)
{

    RLshader  frameShaderId = CompileShader(frameShader, RL_FRAME_SHADER);
	RLshader  vertexShaderId = CompileShader(vertexShader, RL_VERTEX_SHADER);
	RLshader  rayShaderId = CompileShader(rayShader, RL_RAY_SHADER);

	mProgramObject = rlCreateProgram();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");

    rlAttachShader(mProgramObject, frameShaderId);
	rlAttachShader(mProgramObject, vertexShaderId);
	rlAttachShader(mProgramObject, rayShaderId);

    //rlBindAttribLocation(mProgramObject, ATTRIB_POSITION, "InPosition");
    //rlBindAttribLocation(mProgramObject, ATTRIB_NORMAL, "InNormal");
    //rlBindAttribLocation(mProgramObject, ATTRIB_TEXTURE_COORD, "InTexCoord");

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

	return;
}

RLShader::~RLShader()
{
    if (mIsValid)
        rlDeleteProgram(mProgramObject);
}

void RLShader::Bind()
{
	GCLAssert(mIsValid);
	rlUseProgram(mProgramObject);
}


RLshader RLShader::CompileShader(const char *shaderSrc, RLenum type)
{
 
    const std::string fullFilename = std::string(RLSL_PATH) + std::string(shaderSrc);
    char *fileContent = LoadShader(fullFilename.c_str());
	RLshader  shader=0;
	RLint compiled;

	// Create the shader object
	shader = rlCreateShader(type); 

	GCLAssert(shader != 0 && "glCreateShader(type);");

	// Load the shader source
	rlShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	rlCompileShader(shader); 
    delete [] fileContent;
	// Check the compile status
	rlGetShaderiv(shader, RL_COMPILE_STATUS, &compiled); 

	if(compiled == RL_FALSE)
	{
        const char* l;
        rlGetShaderString(shader, RL_COMPILE_LOG, &l);

		std::string log = "RLShader Compile Log for " + std::string(shaderSrc) + ": " ;
		log += l;

        rlDeleteShader(shader);
		GCLAssertMsg(false, log);

	}

	return shader;
}

void RLShader::PrintInfoLog(RLprogram p)
{
	RLint infoLen = 0;
	rlGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);glErrorCheck();
	if(infoLen > 1)
	{
		const char* infoLog;
		rlGetProgramString(p, RL_LINK_LOG, &infoLog);
		std::string linkLog =  std::string("Error linking program:\n") + infoLog ;
        delete [] infoLog;
        throw GCLException(linkLog);
		
	}
}


void RLShader::SetProjectionMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	RLint projectionMatrixLoc = rlGetUniformLocation(mProgramObject,"ProjectionMatrix");
	Matrix44f mf(m);
	rlUniformMatrix4fv(projectionMatrixLoc,1,false,(const RLfloat*)&mf);
}
void RLShader::SetModelViewMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	RLint modelviewMatrixLoc = rlGetUniformLocation(mProgramObject,"ModelViewMatrix");
	Matrix44f mm(m);
	rlUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);
}



void RLShader::SetTextureSampler(const RLTexture &sampler)
{
	RLint textureLoc = rlGetUniformLocation(mProgramObject,"texture");
	rlUniform1i(textureLoc, sampler.GetTextureUnit());
}

void RLShader::GetUniform(const char *uniformName, Matrix44 &m44) const
{
	RLfloat mf[16];
	GCLAssert(mIsValid);
	RLint uniformLoc = rlGetUniformLocation(mProgramObject,uniformName);
	GCLAssert(uniformLoc!=-1);
	rlGetUniformfv(	mProgramObject,uniformLoc,mf);
	m44 = Matrix44((const float *)mf);
}
void RLShader::GetUniform(const char *uniformName, int &sampler) const
{
	GCLAssert(mIsValid);
	RLint uniformLoc = rlGetUniformLocation(mProgramObject,uniformName);
	GCLAssert(uniformLoc!=-1);
	rlGetUniformiv(mProgramObject,uniformLoc,&sampler);
}
int RLShader::GetAttributeLocation(const char *attributeName) const
{
    int ret=-1;

	ret =  (int)rlGetAttribLocation(mProgramObject,attributeName);
	GCLAssertMsg(ret!=-1, attributeName);

	return ret;
}
void RLShader::ResetDefault()
{
	rlUseProgram(0);
}
