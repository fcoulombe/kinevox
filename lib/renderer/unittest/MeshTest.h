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

#include <sstream>
#include <gcl/UnitTest.h>
#include <renderer/Mesh.h>
#include <renderer/MeshResourceManager.h>

using namespace GCL;
namespace MeshTest
{

class MeshRenderObject : public RenderObject
{
public:
	MeshRenderObject()
	: RenderObject("MyRenderObject", Matrix44(true)), //identity
	  mMesh(MESH_PATH"ExampleMesh.mesh")
	{
		mVertexData.mVertexCount =mMesh.GetVertexCount();
		mVertexData.mVertexType = mMesh.GetVertexType();
		mVertexData.mVertexData = mMesh.GetVertexData();
	}

	const VertexData &GetVertexData() const
	{
		return mVertexData;
	}
	const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;
	Mesh mMesh;
	VertexData mVertexData;
};

void Test();
void Test()
{
	TEST_START

	TextureResourceManager::Initialize();
	MeshResourceManager::Initialize();

	GLRenderer render;
	std::stringstream s;
	{
		Mesh mesh(MESH_PATH"ExampleMesh.mesh");
		const void * data = mesh.GetVertexData();
		Assert_Test(data);
		VertexComponents vertexType = mesh.GetVertexType();
		Assert_Test(vertexType == (VertexComponents)(ePOSITION|eTEXTURE_COORD));
		size_t count = mesh.GetVertexCount();
		s<<count << " == " << 21930<<std::endl;
		AssertMsg_Test(count == 21930, s.str().c_str()); //cube

		/*const WorldPoint3 *vertexData = (const WorldPoint3 *)(data);
		for (size_t i=0; i<count; ++i)
		{
			std::cout << "indice: " << i << " " << vertexData[i] << std::endl;
		}*/

	}
	{
		MeshRenderObject myMesh;
		RenderObjectList renderList;
		renderList.push_back(&myMesh);
		render.PreRender();
		render.Render(renderList);
		render.PostRender();
	}


	MeshResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
}
