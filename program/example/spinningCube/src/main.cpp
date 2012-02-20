
#include <unistd.h>
#include <renderer/OpenCV.h>
#include <SDL.h>

#include <applayer/GCLRenderObject.h>
#include <applayer/GCLApplication.h>
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

