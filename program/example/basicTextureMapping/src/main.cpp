
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



		while (true)
		{
			GCLApplication::Update();
			if (Input::IsKeyUp(SDLK_UP))
				myCamera.MoveForward();
			if (Input::IsKeyUp(SDLK_DOWN))
				myCamera.MoveBackward();

			if (Input::IsKeyUp(SDLK_LEFT))
				myCamera.TurnLeft();
			if (Input::IsKeyUp(SDLK_RIGHT))
				myCamera.TurnRight();

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

