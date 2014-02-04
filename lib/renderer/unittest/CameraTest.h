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
#include <iomanip>

#include <3rdparty/OpenGL.h>
#include <kinetestlib/UnitTest.h>

#include <renderer/Camera.h>
#include <renderer/Renderer.h>
#include <windriver/WinDriver.h>

using namespace GCL;
namespace CameraTest
{

    void Test();
void Test()
{
	KINEVOX_TEST_START

    WinDriver winDriver("CameraTest");
	Renderer renderer(winDriver.GetWindowsHandle());

	Camera camera;
	renderer.SetCamera(camera);
	std::stringstream s;
	//camera projection equal Matrix44::Projection
	{
		Assert_Test(abseq(camera.GetFov(), 45.0));
		Assert_Test(abseq(camera.GetAspectRatio(), 640.0/480.0));
		Assert_Test(abseq(camera.GetNear(), 0.1));
		Assert_Test(abseq(camera.GetFar(), 100.0));

		Assert_Test(Matrix44::IDENTITY == camera.GetTransform());
		Matrix44 mview;
		renderer.GetModelView(mview);
		Assert_Test(Inverse(Matrix44::IDENTITY) == mview);

		Matrix44 perspective;
		perspective.SetPerspective( 45.0,640.0/480.0,0.1,100.0);
		s.str("");
		Matrix44 project;
		renderer.GetProjection(project);
		s <<std::setprecision(16)<< std::endl<<perspective << std::endl << "==" << std::endl << project;
		AssertMsg_Test(perspective==project, s.str().c_str());
	}


	ViewPort viewport;

	renderer.SetCamera(camera);

	//std::cout << *(Matrix44*)&camera << std::endl;
	renderer.SetCamera(camera);
	camera.MoveForward();
	renderer.SetCamera(camera);
	camera.MoveBackward();
	renderer.SetCamera(camera);
	camera.TurnLeft();
	renderer.SetCamera(camera);
	camera.TurnRight();
	renderer.SetCamera(camera);
	camera.TiltUp();
	renderer.SetCamera(camera);
	camera.TiltDown();
	renderer.SetCamera(camera);
	KINEVOX_TEST_LOOP_START

		renderer.SetCamera(camera);
		renderer.PreRender();

		renderer.PostRender();
		winDriver.SwapBuffer();
	KINEVOX_TEST_LOOP_END
	//Camera::DefaultCamera.Update();
}
}
