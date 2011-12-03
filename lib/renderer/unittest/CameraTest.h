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

#include <gcl/UnitTest.h>

#include <renderer/Camera.h>
#include <renderer/GLRenderer.h>
#include <renderer/OpenGL.h>
#include <renderer/ViewPort.h>

using namespace GCL;
namespace CameraTest
{

void Test()
{
	TEST_START

	GLRenderer renderer;

	Camera camera;
	std::stringstream s;
	//camera projection equal Matrix44::Projection
	{
		Assert_Test(Matrix44::IDENTITY == camera.GetTransform());
		Assert_Test(Inverse(Matrix44::IDENTITY) == camera.GetModelView());

		Matrix44 perspective;
		perspective.SetPerspective( 45.0,640.0/480.0,0.1,100.0);
		s.str("");
		s <<std::setprecision(16)<< std::endl<<perspective << std::endl << "==" << std::endl << camera.GetProjection();
		AssertMsg_Test(perspective==camera.GetProjection(), s.str().c_str());
	}

	//camera projection equal gluPerspective projection
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,640.0/480.0,0.1,100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		s.str("");
		s<<std::setprecision(16)<< std::endl<<GLRenderer::GetGLProjection() << std::endl <<"==" << std::endl << camera.GetProjection();
		AssertMsg_Test(GLRenderer::GetGLProjection() == camera.GetProjection(), s.str().c_str());
	}



	renderer.SetCamera(camera);
	ViewPort viewport;

	camera.Update();




	//std::cout << *(Matrix44*)&camera << std::endl;
	camera.Update();
	camera.MoveForward();
	camera.Update();
	camera.MoveBackward();
	camera.Update();
	camera.TurnLeft();
	camera.Update();
	camera.TurnRight();
	camera.Update();
	camera.TiltUp();
	camera.Update();
	camera.TiltDown();
	camera.Update();
	//Camera::DefaultCamera.Update();
}
}
