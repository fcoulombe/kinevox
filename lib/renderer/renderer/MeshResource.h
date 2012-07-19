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

#pragma once

#include <3rdparty/OpenGL.h>
#include <gcl/Point3.h>
#include <gcl/Resource.h>

namespace GCL
{
class GCLFile;
//this is a resource class that can be shared between difference Mesh instance
class MeshResource : public Resource
{
public:

	MeshResource(const char *MeshName);
	~MeshResource();

	struct MaterialData
	{
		//WorldPoint4 mDiffuse;
		//WorldPoint4 mAmbient;
		//WorldPoint4 mEmissive;
		Real mShininess;
	};
	struct MeshData
	{
		MeshData()
		: mVertexCount(0),
		  mIndicesCount(0),
		  mNormalCount(0),
		  mVertexColorCount(0),
		  mMaterialCount(0),
		  mUvCount(0)
		{}
		~MeshData(){}

		uint32_t mVertexCount;
		uint32_t mIndicesCount;

		uint32_t mNormalCount;
		uint32_t mVertexColorCount;

		uint32_t mMaterialCount;
		uint32_t mUvCount;
		uint8_t pad[4];
		uint32_t mMaterialNameLen; //the name len is part of the string write up

		const char *GetMaterialName() const { return (const char*)(this)+sizeof(MeshData); }
		const void *GetVertexData() const { return (uint8_t*)(this)+sizeof(MeshData)+mMaterialNameLen; }

	};
	const MeshData *mMeshData;

	static const MeshResource EmptyMesh;

	static const MeshData *LoadMesh(GCLFile &is);
	static void Unload(const MeshData *data);
private:
	MeshResource() {}
};
}
