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
#include <applayer/Actor.h>
#include <applayer/GCLApplication.h>


using namespace GCL;
namespace ScriptComponentTest
{

void Test()
{
	KINEVOX_TEST_START
	GCLApplication::Initialize("ScriptComponentTest");
	{
	Actor actor("TestScriptedActor", "TestScriptedActor");
	actor.SetPosition(0.0,0.0,-10.0);
	Actor actor2("TestScriptedActor", "TestScriptedActor");
	actor2.SetPosition(0.0,0.0,-10.0);

	Camera cam;
	KINEVOX_TEST_LOOP_START
		cam.Update();
		actor.Update(1.0);
		actor2.Update(1.0);
		GCLApplication::Update();
		GCLApplication::Render();
	KINEVOX_TEST_LOOP_END
	}
	GCLApplication::Terminate();
}
}
