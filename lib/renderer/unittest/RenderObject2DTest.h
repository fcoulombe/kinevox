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

#include <gcl/UnitTest.h>
#include <renderer/RenderObject2D.h>
#include <renderer/TextureResourceManager.h>

using namespace GCL;
namespace RenderObject2DTest
{
class MyRenderObject2D : public RenderObject2D
{
public:
	MyRenderObject2D()
	: RenderObject2D("DefaultSprite") //identity
	{}
private:
};

    void Test();
void Test()
{
	TEST_START
	TextureResourceManager::Initialize();
	GLRenderer renderer;
	MyRenderObject2D obj;
	RenderObject2DList objList;
	objList.push_back(&obj);

	std::stringstream s;

	//test setposition
	{
	const WorldPoint3 position(1.0,0.0, 0.0);
	obj.SetPosition(position);

	s.str("");
	s<<position << "\n==\n" << obj.GetPosition() << std::endl;
	AssertMsg_Test(position == obj.GetPosition(), s.str().c_str());
	}

	//test setscale
	{
	const WorldPoint2 scale(2.0,2.0);
	obj.SetScale(scale);
	}

	//set sprite test
	{
		obj.SetSprite("DefaultSprite");
	}
	obj.Update();

	renderer.PreRender();
	renderer.Render(objList);
	renderer.PostRender();

	TextureResourceManager::Terminate();


}
}