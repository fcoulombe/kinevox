
#include <unistd.h>
#include <cv.h>
#include <SDL.h>

#include <AppLayer/GCLRenderObject.h>
#include <Input/Input.h>
#include <Renderer/GLRenderer.h>
#include <Renderer/RenderObject.h>
#include <Renderer/Vertex.h>

using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
  OpenGLRenderer renderer;
  //Camera &tempCamera = renderer.GetCamera();

  GCLRenderObject renderObject;
  renderObject.SetPosition(0,0,-10.0);

  renderer.RegisterRenderObject(renderObject);

  while (true)
    {
#if 0
      ProcessInput();

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

      renderer.Render();

      if (!renderer.Update())
        return 1;
#else
      renderer.Render();
      sleep(0);
    }


#endif
  return 0;
}

