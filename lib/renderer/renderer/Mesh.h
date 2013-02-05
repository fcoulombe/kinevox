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

#include "renderer/Material.h"
#include "renderer/MeshResource.h"
#include "renderer/Vertex.h"

namespace GCL
{
class Material;
class Mesh
{
public:
	Mesh(const char *filename="DefaultMesh");
	~Mesh();
	const void * GetVertexData(size_t subMeshIndex) const { return mMeshResource->mMeshData->GetSubMeshData(subMeshIndex)->GetVertexData(); }
	VertexComponents GetVertexType() const { return (VertexComponents)(ePOSITION|eNORMAL|eTEXTURE_COORD); }
	size_t GetVertexCount(size_t subMeshIndex) const { return mMeshResource->mMeshData->GetSubMeshData(subMeshIndex)->mIndicesCount; }
	const Material &GetMaterial() const { return *mMaterial; }
private:
	Material *mMaterial;
	const MeshResource *mMeshResource;
};

}
