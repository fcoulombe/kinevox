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
#include <kinetestlib/UnitTest.h>
#include <renderer/Mesh.h>
#include <renderer/MeshResourceManager.h>

using namespace GCL;
namespace MeshTest
{

class MeshRenderObject 
{
public:
	MeshRenderObject()
	 : mMesh(MESH_PATH"ExampleMesh.mesh")
	{
        VertexData data;
		data.mVertexCount =mMesh.GetVertexCount(0);
		data.mVertexType = mMesh.GetVertexType();
		data.mVertexData = mMesh.GetVertexData(0);
        mVertexData.push_back(data);
		switch ((size_t)mMesh.GetVertexType())
		{
		case ePOSITION:
			obj = new RenderObject(Matrix44::IDENTITY, mMesh.GetMaterial(), (const VertexP*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		case ePOSITION|eNORMAL:
			obj = new RenderObject(Matrix44::IDENTITY, mMesh.GetMaterial(), (const VertexPN*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		case ePOSITION|eNORMAL|eTEXTURE_COORD:
			obj = new RenderObject(Matrix44::IDENTITY, mMesh.GetMaterial(), (const VertexPNT*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		default:
			GCLAssert(false);
		}
	}
	~MeshRenderObject()
	{
		delete obj;
	}
	void SetPosition(Real x, Real y, Real z) { obj->SetPosition(x,y,z); }

	void SetOrientation(Real x, Real y, Real z) { obj->SetOrientation(x,y,z); }
	RenderObject *GetObj() { return obj; }
private:
	Mesh mMesh;
	RenderObject *obj;
	VertexDataList mVertexData;
};

void Test();
void Test()
{
	KINEVOX_TEST_START

	TextureResourceManager::Initialize();
	MeshResourceManager::Initialize();

    WinDriver windriver("MeshTest");
	Renderer renderer(windriver.GetWindowsHandle());
	std::stringstream s;

	{
		Mesh mesh(MESH_PATH"ExampleMesh.mesh");
		const void * data = mesh.GetVertexData(0);
		Assert_Test(data);
		VertexComponents vertexType = mesh.GetVertexType();
		Assert_Test(vertexType == (VertexComponents)(ePOSITION|eNORMAL|eTEXTURE_COORD));
		size_t count = mesh.GetVertexCount(0);
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
        myMesh.SetPosition(0.0,0.0,-10.0);
		RenderObjectList renderList;
		renderList.push_back(myMesh.GetObj());
        Real rot = 0.0;
        KINEVOX_TEST_LOOP_START
            rot+=0.001;
        myMesh.SetOrientation(0.0,rot,0.0);
		renderer.PreRender();
		renderer.Render(renderList);
		renderer.PostRender();
        windriver.SwapBuffer();
        KINEVOX_TEST_LOOP_END
	}


	MeshResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
}
