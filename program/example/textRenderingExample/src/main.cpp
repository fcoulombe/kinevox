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

#include <applayer/GCLApplication.h>
#include <applayer/GCLRenderObject2D.h>
#include <gcl/Assert.h>
#include <gcl/Exception.h>
#include <gcl/ResourceManagerConfig.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/Camera.h>

#include <windriver/TTFFont.h>

#include <renderer/GLRenderer.h>
#include <windriver/WinDriver.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize();
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		bool isRunning = true;
		Real x = 100.0;
		Real y = 100.0;
		TTFFont font(FONT_PATH"FreeMono.ttf", 12);
		while (isRunning)
		{
			GCLApplication::Update();
			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;

			if (Input::IsKeyUp(SDLK_UP))
				y-=0.1;
			if (Input::IsKeyUp(SDLK_DOWN))
				y+=0.1;

			if (Input::IsKeyUp(SDLK_LEFT))
				x-=0.1;
			if (Input::IsKeyUp(SDLK_RIGHT))
				x+=0.1;
			WinDriver *winDriver = GCLApplication::GetWinDriver();
			GLRenderer *renderer = GCLApplication::GetRenderer();

			   GCLAssert(winDriver);
				GCLAssert(renderer);
				renderer->PreRender();
				//font.DrawText(*winDriver, "HelloWorld", 100, 100);

				//renderer->Render(mRenderObjectList);
			    //const ViewPort &tempViewPort = winDriver->GetViewPort();
			    //renderer->Render(mRenderObject2DList, tempViewPort.GetWidth(), tempViewPort.GetHeight());
			    //renderer->PostRender();
			  //  winDriver->SwapBuffer();
			//GCLApplication::Render();
			Time::SleepMs(1000);
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

