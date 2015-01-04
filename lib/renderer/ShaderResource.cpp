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

#include "renderer/ShaderResource.h"

#include <fstream>
#include <cstring>

#include <gcl/Assert.h>
#include <gcl/File.h>

using namespace GCL;

const ShaderResource ShaderResource::EmptyShader;

char *LoadShader(const char *filename)
{
	GCLAssertMsg(ResourceFile::Exists(filename), filename);

	ResourceFile fp(filename);
	size_t fileSize = fp.GetFileSize();
	char *fileContent = new char[fileSize+1];
	fp.Read(fileContent, fileSize);
	fileContent[fileSize] = 0;
	return fileContent;
}

ShaderResource::ShaderResource( const char *shaderFileName )
: Resource(),
mShaderFileName(shaderFileName)
{
	mShaderSource = LoadShader(shaderFileName);
}

ShaderResource::~ShaderResource()
{
	delete [] mShaderSource;
}


