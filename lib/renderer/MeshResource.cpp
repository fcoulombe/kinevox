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

#include "renderer/MeshResource.h"
#include <gcl/Assert.h>
#include <gcl/File.h>

#include <cstring>


using namespace GCL;

const MeshResource MeshResource::EmptyMesh;

void MeshResource::Unload(const MeshData *data)
{
	GCLAssert(data);
	uint8_t *buffer =(uint8_t *)data;
	delete [] buffer;
}

const MeshResource::MeshData * MeshResource::LoadMesh(GCLFile & fp)
{
	size_t fileSize = fp.GetFileSize();
	uint8_t *buffer = new uint8_t[fileSize];
	const MeshData *data = reinterpret_cast<const MeshData*>(buffer);
	fp.Read(buffer, fileSize);
	return data;
}

//this loads TGA files and then upload it to opengl
MeshResource::MeshResource( const char *MeshName )
: Resource()
{
	mMeshData = NULL;
	std::string path = Resource::GetResourcePath();
	path += MeshName;


	const char *ext = &(path.c_str()[path.length()-4]);
	if (strncmp(ext, "mesh", 4) == 0)
	{
		GCLFile fp(path.c_str());

		mMeshData =LoadMesh(fp);
		GCLAssert(mMeshData);

		fp.Close();
	}
	else
	{
		GCLAssertMsg(false, "this extension is not supported")
	}
}

MeshResource::~MeshResource()
{
	if (mMeshData)
	{
		Unload(mMeshData);
	}
}

