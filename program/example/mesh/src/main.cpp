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

#include <applayer/GCLRenderObject.h>
#include <applayer/GCLApplication.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/Camera.h>
#include <renderer/Material.h>
#include <renderer/GLRenderer.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize("Mesh Example");
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);

		GCLRenderObject renderObject;
		renderObject.SetPosition(0,0,-10.0);

		bool isRunning = true;
		Real x = 0.0;
		Real y = 0.0;
		while (isRunning)
		{
			GCLApplication::Update();
			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;

			if (Input::IsKeyUp(SDLK_UP))
				x+=0.01;
			if (Input::IsKeyUp(SDLK_DOWN))
				x-=0.01;

			if (Input::IsKeyUp(SDLK_LEFT))
				y+=0.01;
			if (Input::IsKeyUp(SDLK_RIGHT))
				y-=0.01;

			if (Input::IsKeyUp(SDLK_PAGEUP))
				myCamera.TiltUp();
			if (Input::IsKeyUp(SDLK_PAGEDOWN))
				//myCamera.TiltDown();
			{
				static bool isTextureOn = false;
				isTextureOn = !isTextureOn;
				if (isTextureOn)
				{
					std::cout << "on" << std::endl;
					GCLApplication::GetRenderer()->mCurrentRenderState.SetTextureEnabled(true);
				}
				else
				{
					std::cout << "off" << std::endl;
					GCLApplication::GetRenderer()->mCurrentRenderState.SetTextureEnabled(false);
				}
			}


			//x +=0.01;
			//y+=0.01;
			renderObject.SetOrientation(x,y,0.0);

			std::cout << renderObject.GetTransform() << std::endl;

			GCLApplication::Render();
			Time::SleepMs(100);
			std::cout.flush();
		}
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	GCLApplication::Terminate();

	return 0;
}

