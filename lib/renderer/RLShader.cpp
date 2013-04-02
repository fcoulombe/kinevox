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
#include <gcl/ResourceManagerConfig.h>



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
RLShader::RLShader(const char *shaderFileName , RLenum type)
:mIsValid (false)
{
    mShader = CompileShader(shaderFileName, type);
}

RLShader::~RLShader()
{
    rlDeleteShader(mShader);
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
	rlShaderSource(shader, 1, &fileContent, NULL);

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
    else
    {
        mIsValid = true;
    }

	return shader;
}
