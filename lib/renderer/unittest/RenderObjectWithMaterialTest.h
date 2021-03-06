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

using namespace GCL;
namespace RenderObjectWithMaterialTest
{
void Test();
void Test()
{
	KINEVOX_TEST_START

		ShaderResourceManager::Initialize();
	TextureResourceManager::Initialize();
	{
		WinDriver winDriver("RenderObjectWithMaterialTest");
		Renderer renderer(winDriver.GetWindowsHandle());

		SquareRenderObject obj;
		RenderObjectList objList;

		objList.push_back(obj.GetRenderObject());

		Material material("Default");
		material.Bind();
        obj.SetPosition(0.0,0.0,-10.0);
        Real rot = 0.0;
        KINEVOX_TEST_LOOP_START
        rot+=0.001;
        obj.SetOrientation(0.0,rot,0.0);
		renderer.PreRender();
		Matrix44 proj;
		renderer.GetProjection(proj);
		obj.Render(proj);
		renderer.PostRender();
		winDriver.SwapBuffer();
        KINEVOX_TEST_LOOP_END
	}
	TextureResourceManager::Terminate();
	ShaderResourceManager::Terminate();
}
}
