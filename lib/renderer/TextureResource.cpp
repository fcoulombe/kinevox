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

#include "renderer/TextureResource.h"

#include <fstream>
#include <cstring>

#include <gcl/Assert.h>
#include <gcl/File.h>

using namespace GCL;

const TextureResource TextureResource::EmptyTexture;



TextureResource::TextureResource( const char *textureName )
: Resource()
{
	std::string path = Resource::GetResourcePath();
	path += textureName;

	std::string msg("trying to loads " );
	msg += path.c_str();
	msg += "\n";

	const std::string ext = Path::GetFileNameExtension(path);
	TextureData &data = mTextureData;
	if (strncmp(ext.c_str(), "tga", 3) == 0)
	{
		GCLFile fp(path.c_str());
		PixelBuffer::LoadTga(fp, data.imageData);
		GCLAssert(data.imageData.mPixels);
	}
	else if (strncmp(ext.c_str(), "png", 3) == 0)
	{
#ifndef OS_IPHONE
		GCLFile fp(path.c_str());

		PixelBuffer::LoadPng(fp, data.imageData);
		GCLAssert(data.imageData.mPixels);
#else
		GCLAssertMsg(false, "cant load png on iphone yet" );
#endif

	}
	else if (strncmp(ext.c_str(), "raw", 3) == 0)
	{
		GCLAssert(false && "can't load a raw texture like this since we dont have information about dimensions");
	}
	else
	{
		std::stringstream s;
		s<<"this extension is not supported: "<<ext;
		GCLAssertMsg(false, s.str().c_str());
	}
}

TextureResource::~TextureResource()
{
}

TextureResource::TextureResource(const PixelBuffer &pixelBuffer)
{
	TextureData &data = mTextureData;
	data.imageData.Initialize(pixelBuffer);
}


