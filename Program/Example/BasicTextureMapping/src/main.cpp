
#include <unistd.h>
#include <cv.h>
#include <SDL.h>

#include <AppLayer/GCLRenderObject.h>
#include <AppLayer/GCLApplication.h>
#include <Input/Input.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		std::cout << "render" << std::endl;
		GCLApplication::Initialize();
		//Camera &tempCamera = renderer.GetCamera();

		std::cout << "obj" << std::endl;
		GCLRenderObject renderObject;
		//renderObject.SetPosition(0,0,-10.0);


		while (true)
		{
			std::cout << "kllooop" << std::endl;
			GCLApplication::Update();
#if 0
			if (IsKeyUp(SDLK_UP))
				tempCamera.MoveForward();
			if (IsKeyUp(SDLK_DOWN))
				tempCamera.MoveBackward();

			if (IsKeyUp(SDLK_LEFT))
				tempCamera.TurnLeft();
			if (IsKeyUp(SDLK_RIGHT))
				tempCamera.TurnRight();

			if (IsKeyUp(SDLK_PAGEUP))
				tempCamera.TiltUp();
			if (IsKeyUp(SDLK_PAGEDOWN))
				tempCamera.TiltDown();
#else

			GCLApplication::Render();
			sleep(1);
		}
		#endif
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}


	GCLApplication::Terminate();

	return 0;
}

