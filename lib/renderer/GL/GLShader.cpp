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
#include <iostream>
#include "renderer/GL/GLShader.h"
#include <gcl/Exception.h>
#include <gcl/File.h>


using namespace GCL;

const GLenum GLShader::GLShaderType[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER
};

const char *SHADER_HEADER =
		"#version 150 core	\n"
		"#pragma optimize(off) \n"
		"#pragma debug(on) \n"
		"\n";

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
GLShader::GLShader(const char *shaderSourcePath, GLenum type)
:mIsValid (false)
{
#if ENABLE_SHADERS
	mShaderObject = CompileShader(shaderSourcePath, type);

#endif
	return;
}

GLShader::~GLShader()
{
#if ENABLE_SHADERS
    if (mIsValid)
        glDeleteShader(mShaderObject);glErrorCheck();
#endif
}


GLuint GLShader::CompileShader(const char *shaderPath, GLenum type)
{
#if  ENABLE_SHADERS
#if USE_OPENGL3
	const std::string fullFilename = std::string(GLSL_PATH) + std::string(shaderPath)+std::string(".glsl3");
#else
	const std::string fullFilename = std::string(GLSL_PATH) + std::string(shaderPath)+std::string(".glsl");
#endif
	char *fileContent = LoadShader(fullFilename.c_str());


	std::string buffer(SHADER_HEADER);
	buffer += fileContent;


	GLuint shader=0;
	shader = glCreateShader(type); glErrorCheck();
	GCLAssert(shader != 0 && "glCreateShader(type);");
	const GLchar *cbuffer = (const GLchar *)buffer.c_str();
	glShaderSource(shader, 1, &cbuffer, NULL); glErrorCheck();
	glCompileShader(shader); glErrorCheck();
	delete [] fileContent;
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
			log += fullFilename;
			log += infoLog;

			delete [] infoLog;
			glDeleteShader(shader);glErrorCheck();
			GCLAssertMsg(false, log);
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

void GLShader::PrintInfoLog(GLuint p)
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


