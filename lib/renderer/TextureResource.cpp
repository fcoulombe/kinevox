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
#include <iostream>
#include <cstring>

#include "gcl/Assert.h"

using namespace GCL;

const TextureResource TextureResource::EmptyTexture;

void TextureResource::Unload(TextureData &data)
{
	if (data.imageData)
		delete [] data.imageData;
}


TextureResource::TextureResource( const char *textureName )
: Resource()
{
	std::string path = Resource::GetResourcePath();
	path += textureName;

	std::string msg("trying to loads " );
	msg += path.c_str();
	msg += "\n";

	const char *ext = &(path.c_str()[path.length()-3]);
	TextureData &data = mTextureData;
	if (strncmp(ext, "tga", 3) == 0)
	{
		std::fstream fp(path.c_str(), std::fstream::binary|std::fstream::in);
		GCLAssertMsg(fp.is_open() && fp.good(), msg.c_str());

		LoadTga(fp, data);
		GCLAssert(data.imageData);

		fp.close();
	}
	else if (strncmp(ext, "png", 3) == 0)
	{
#ifndef OS_IPHONE
		FILE *fp = fopen(path.c_str(), "rb");
		GCLAssertMsg(fp, path.c_str());

		LoadPng(fp, data);
		GCLAssert(data.imageData);
		fclose(fp);
#else
		GCLAssertMsg(false, "cant load png on iphone yet" );
#endif

	}
	else if (strncmp(ext, "raw", 3) == 0)
	{
		GCLAssert(false && "can't load a raw texture like this since we dont have information about dimensions");
	}
	else
	{
		GCLAssertMsg(false, "this extension is not supported")
	}
}

TextureResource::~TextureResource()
{
	Unload(mTextureData);

}


