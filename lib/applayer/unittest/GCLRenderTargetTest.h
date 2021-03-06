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

#include <applayer/GCLApplication.h>
#include <applayer/GCLRenderTarget.h>

#include <kinetestlib/UnitTest.h>

using namespace GCL;
namespace GCLRenderTargetTest
{


void Test()
{
	KINEVOX_TEST_START

	GCLApplication::Initialize("GCLRenderTargetTest");
	{
	Actor obj("TestActor", "TestActor");
	Assert_Test(GCLApplication::IsRegistered(obj));
	Assert_Test(obj.GetTransform() == Matrix44::IDENTITY);

	const WorldPoint3 position(0.0,0.0, -10.0);
	obj.SetPosition(position);

	GCLRenderTarget target(512,512);
    KINEVOX_TEST_LOOP_START
		GCLApplication::Update();
		GCLApplication::Render();
    KINEVOX_TEST_LOOP_END
	target.Save("GCLRenderTargetTest.tga");
	}
	GCLApplication::Terminate();
}
}
