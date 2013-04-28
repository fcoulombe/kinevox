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
#include <renderer/GeomUtilHelper.h>
#include <renderer/Material.h>
#include <renderer/RenderObject.h>
#include <renderer/Shader.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace RenderObjectTest
{

class MyRenderObject : public SquareRenderObject
{
public:
	MyRenderObject()
	: SquareRenderObject("MyRenderObject", Matrix44(true)) //identity
	{
    }

	const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;
};


void Test();
void Test()
{
	KINEVOX_TEST_START
	TextureResourceManager::Initialize();
	{
		WinDriver winDriver("RenderObjectTest");
		Renderer renderer(winDriver.GetWindowsHandle());
		MyRenderObject obj;
		RenderObjectList objList;
		objList.push_back(&obj);

		std::stringstream s;
		//test initial transform is identity
		{
			const Matrix44 &transform = obj.GetTransform();
			s<<std::endl<<transform<<std::endl<<"=="<<std::endl<<Matrix44::IDENTITY;
			AssertMsg_Test(transform == Matrix44::IDENTITY, s.str().c_str());
		}

		//test settransform
		{
			obj.SetTransform(Matrix44::IDENTITY);
			s.str("");
			s<<std::endl<<obj.GetTransform()<<std::endl<<"=="<<std::endl<<Matrix44::IDENTITY;
			AssertMsg_Test(obj.GetTransform() == Matrix44::IDENTITY, s.str().c_str());
		}
		//test setposition
		{
			const WorldPoint3 position(0.0,0.0, -10.0);
			obj.SetPosition(position);
			Matrix44 positionTestMat(Matrix44::IDENTITY);
			positionTestMat.SetPosition(position);
			s.str("");
			s<<positionTestMat << "\n==\n" << obj.GetTransform() << std::endl;
			AssertMsg_Test(positionTestMat == obj.GetTransform(), s.str().c_str());

			obj.SetTransform(Matrix44::IDENTITY);
		}

		//test orientation
		{
			obj.SetOrientation(90.0,0.0,0.0);
			Matrix44 rotationTestMat;
			rotationTestMat.SetRotationX(90.0);
			s.str("");
			s<<rotationTestMat << "\n==\n" << obj.GetTransform() << std::endl;
			AssertMsg_Test(rotationTestMat == obj.GetTransform(), s.str().c_str());
			obj.SetTransform(Matrix44::IDENTITY);
		}

		//test concat position and orientation
		{
			obj.SetOrientation(90.0,0.0,0.0);
			obj.SetPosition(10.0,0.0,0.0);
			Matrix44 concatTestMat;
			concatTestMat.SetRotationX(90.0);
			concatTestMat.SetPosition(WorldPoint3(10.0,0.0,0.0));
			s.str("");
			s<<concatTestMat << "\n==\n" << obj.GetTransform() << std::endl;
			AssertMsg_Test(concatTestMat == obj.GetTransform(), s.str().c_str());
			obj.SetTransform(Matrix44::IDENTITY);
		}
		//test concat2 position and orientation
		{

			obj.SetPosition(10.0,0.0,0.0);
			obj.SetOrientation(90.0,0.0,0.0);

			Matrix44 concatTestMat;
			concatTestMat.SetRotationX(90.0);
			concatTestMat.SetPosition(WorldPoint3(10.0,0.0,0.0));
			s.str("");
			s<<concatTestMat << "\n==\n" << obj.GetTransform() << std::endl;
			AssertMsg_Test(concatTestMat == obj.GetTransform(), s.str().c_str());
			obj.SetTransform(Matrix44::IDENTITY);

		}
        obj.SetPosition(0.0,0.0,-10.0);
        Real rot = 0.0;
        KINEVOX_TEST_LOOP_START
        rot+=0.001;
        obj.SetOrientation(0.0,rot,0.0);
		renderer.PreRender();
		renderer.Render(objList);
		renderer.PostRender();
		winDriver.SwapBuffer();
        KINEVOX_TEST_LOOP_END
	}
	TextureResourceManager::Terminate();


}
}
