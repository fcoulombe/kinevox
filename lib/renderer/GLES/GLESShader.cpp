/*
 * Copyright (C) 2011 by Francois Coulombe
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

#include "renderer/GLES/GLESShader.h"
#include <gcl/Exception.h>
#include <gcl/Matrix44.h>

#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/Texture.h"

using namespace GCL;


const char *SHADER_HEADER =
		"#version 110	\n"
		"#pragma optimize(off) \n"
		"#pragma debug(on) \n"
		"\n";

GLESShader::GLESShader()
:mIsValid (false)
{
	std::string vbuffer(pszVertShader);
	//vbuffer += DefaultVShaderStr;
	std::string fbuffer(pszFragShader);
	//fbuffer += DefaultFShaderStr;
#if ENABLE_SHADERS
	GLuint vertexShader = CompileShader(vbuffer.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fbuffer.c_str(), GL_FRAGMENT_SHADER);

	mProgramObject = glCreateProgram();glErrorCheck();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");

	glAttachShader(mProgramObject, vertexShader);glErrorCheck();
	glAttachShader(mProgramObject, fragmentShader);glErrorCheck();

    glBindAttribLocation(mProgramObject, ATTRIB_POSITION, "InPosition");glErrorCheck();
    glBindAttribLocation(mProgramObject, ATTRIB_NORMAL, "InNormal");glErrorCheck();
    glBindAttribLocation(mProgramObject, ATTRIB_TEXTURE_COORD, "InTexCoord");glErrorCheck();

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
#endif
	return;
}

GLESShader::~GLESShader()
{
#if ENABLE_SHADERS
    if (mIsValid)
        glDeleteProgram(mProgramObject);glErrorCheck();
#endif
}

void GLESShader::Bind()
{
#if ENABLE_SHADERS
	GCLAssert(mIsValid);
	glUseProgram(mProgramObject);glErrorCheck();
#endif
}


GLuint GLESShader::CompileShader(const char *shaderSrc, GLenum type)
{
    
	GLuint shader=0;
#if  ENABLE_SHADERS
	

	// Create the shader object
	shader = glCreateShader(type); glErrorCheck();

	GCLAssert(shader != 0 && "glCreateShader(type);");

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL); glErrorCheck();

	// Compile the shader
	glCompileShader(shader); glErrorCheck();
	// Check the compile status
    GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus); glErrorCheck();

	if(!compileStatus)
	{
		GLint infologLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);glErrorCheck();
		if (infologLength > 0)
		{
			GLchar *infoLog = new GLchar[infologLength];
			int charsWritten  = 0;
			glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);

			std::string log = "Shader Compile Log: " ;
			log += infoLog;

			delete [] infoLog;

			throw GCLException(log);
		}
		glDeleteShader(shader);glErrorCheck();
		return 0;
	}
#else
	(void)type;
	(void)shaderSrc;
#endif
	return shader;
}

void GLESShader::PrintInfoLog(GLuint p)
{
#if ENABLE_SHADERS
	GLint infoLen = 0;
	glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);glErrorCheck();
	if(infoLen > 1)
	{
		char* infoLog = new char[(sizeof(char) * infoLen)];
		glGetProgramInfoLog(p, infoLen, NULL, infoLog);glErrorCheck();
		std::cerr << "Error linking program:\n%s" << infoLog << std::endl;
		delete [] infoLog;
	}
#else
	(void)p;
#endif

}




void GLESShader::SetProjectionMatrix(const Matrix44 &m)
{
#if ENABLE_SHADERS
	GCLAssert(mIsValid);
	GLint projectionMatrixLoc = glGetUniformLocation(mProgramObject,"ProjectionMatrix");glErrorCheck();
	Matrix44f mf(m);
	glUniformMatrix4fv(projectionMatrixLoc,1,false,(const GLfloat*)&mf);glErrorCheck();
#else
	(void)m;
#endif
}
void GLESShader::SetModelViewMatrix(const Matrix44 &m)
{
#if ENABLE_SHADERS
	GCLAssert(mIsValid);
	GLint modelviewMatrixLoc = glGetUniformLocation(mProgramObject,"ModelViewMatrix");glErrorCheck();
	Matrix44f mm(m);
	glUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);glErrorCheck();
#else
	(void)m;
#endif
}



void GLESShader::SetTextureSampler(const GLESTexture &sampler)
{
    #if ENABLE_SHADERS
	GLint textureLoc = glGetUniformLocation(mProgramObject,"texture");glErrorCheck();
	glUniform1i(textureLoc, sampler.GetTextureUnit());glErrorCheck();
#else
	(void)sampler;
#endif
}

void GLESShader::GetUniform(const char *uniformName, Matrix44 &m44) const
{
    #if ENABLE_SHADERS
	GLfloat mf[16];
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformfv(	mProgramObject,uniformLoc,mf);glErrorCheck();
	m44 = Matrix44((const float *)mf);
#else
	(void)uniformName;
	(void)m44;
#endif
}
void GLESShader::GetUniform(const char *uniformName, int &sampler) const
{
    #if ENABLE_SHADERS
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformiv(mProgramObject,uniformLoc,&sampler);glErrorCheck();
#else
	(void)uniformName;
	(void)sampler;
#endif
}
int GLESShader::GetAttributeLocation(const char *attributeName) const
{
    int ret=-1;
    #if ENABLE_SHADERS
	ret =  (int)glGetAttribLocation(mProgramObject,attributeName);glErrorCheck();
	GCLAssertMsg(ret!=-1, attributeName);
#else
	(void)attributeName;
	#endif

	return ret;
}
void GLESShader::ResetDefault()
{
#if ENABLE_SHADERS
	glUseProgram(0);glErrorCheck();
#endif
}
