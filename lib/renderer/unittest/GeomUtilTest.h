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

#include <kinetestlib/UnitTest.h>
#include <renderer/GeomUtil.h>
#include <renderer/GeomUtilHelper.h>

using namespace GCL;
namespace GeomUtilTest
{

bool CompareImages(const char * /*filename1*/, const char * /*filename2*/);
bool CompareImages(const char * /*filename1*/, const char * /*filename2*/)
{
	return false;
}
void Test();

void Test()
{
	KINEVOX_TEST_START
	TextureResourceManager::Initialize();

	{
		std::vector<WorldPoint3> dst;
		Real radius = 1.0;
		GeomUtil::MakeMeshSphere(dst, radius);
		GeomUtil::MakeMeshCircle(dst, radius);

		std::vector<WorldPoint3> vertexData;
        std::vector<WorldPoint3> normalData;
        std::vector<WorldPoint2> tcoordData;
		Real size = 1.0;
		GeomUtil::MakeMeshPlane(vertexData,tcoordData,size);
		GeomUtil::MakeMeshCube(vertexData,normalData,tcoordData,size);

	}

	{
		WinDriver winDriver("GeomUtilTest");
		Renderer renderer(winDriver.GetWindowsHandle());

        RenderObjectList objList;
		CubeRenderObject cube;
        cube.SetPosition(3.0,0.0,-8.0);
        cube.SetEnableDrawNormals();
		objList.push_back(cube.GetRenderObject());
        SphereRenderObject sphere;
        sphere.SetPosition(1,0.0,-8.0);
        objList.push_back(sphere.GetRenderObject());
        PlaneRenderObject plane;
        plane.SetPosition(-1,0.0,-8.0);
        objList.push_back(plane.GetRenderObject());
        CircleRenderObject circle;
        circle.SetPosition(-3.0,0.0,-8.0);
        objList.push_back(circle.GetRenderObject());


		try
		{
            Real rot = 0.0;
            KINEVOX_TEST_LOOP_START
                rot+=0.001;
            cube.SetOrientation(0.0,rot,0.0);
            circle.SetOrientation(0.0,rot,0.0);
            sphere.SetOrientation(0.0,rot,0.0);
            plane.SetOrientation(0.0,rot,0.0);
            renderer.PreRender();
            renderer.Render(objList);
            renderer.PostRender();
            winDriver.SwapBuffer();
            KINEVOX_TEST_LOOP_END
		}
		catch (GCLException &e)
		{
			AssertMsg_Test(false, e.what());
		}
	}

	TextureResourceManager::Terminate();
}
}
