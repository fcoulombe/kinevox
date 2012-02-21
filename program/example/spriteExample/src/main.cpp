
#include <unistd.h>
#include <renderer/OpenCV.h>
#include <SDL.h>

#include <applayer/GCLSprite.h>
#include <applayer/GCLApplication.h>
#include <gcl/Exception.h>
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
		GCLApplication::Initialize();
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		GCLSprite obj;


		bool isRunning = true;
		Real x = 0.0;
		Real y = 0.0;
		while (isRunning)
		{
			GCLApplication::Update();
			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;

			if (Input::IsKeyUp(SDLK_UP))
				y+=0.01;
			if (Input::IsKeyUp(SDLK_DOWN))
				y-=0.01;

			if (Input::IsKeyUp(SDLK_LEFT))
				x-=0.01;
			if (Input::IsKeyUp(SDLK_RIGHT))
				x+=0.01;

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


			obj.SetPosition(WorldPoint3(x,y,0.0));
			GCLApplication::Render();
			usleep(100);
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

