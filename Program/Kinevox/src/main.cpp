
#include <unistd.h>
#include <cv.h>

#include "Input/Input.h"
#include "Kinect/KinectDriver.h"
#include "Renderer/GLRenderer.h"
//#include "Voxel/Grid.h"

using namespace GCL;

int use_nested_cascade = 0;


#include <SDL.h>
#include <Renderer/OpenGL.h>
int main(int argc, char **argv)
{
  /*if (!InitKinect())
		return -1;*/

  OpenGLRenderer *renderer = new OpenGLRenderer();

  IplImage* rgbImg;
  IplImage* depthImg;
  rgbImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
  depthImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);

  //	Grid grid(10,10,10);

  //	renderer->RegisterRenderObject(&grid);
  while (true)
    {
#if 0
      ProcessInput();
      if (!SynchronizeDrawingData())
        continue;


      cvSetData(rgbImg, GetKinectRgbData(), 640*3);
      cvSetData(depthImg, GetKinectDepthData(), 640*3);

      Camera &tempCamera = renderer->GetCamera();
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



      if(IsKeyUp(306)) { //left ctrl
          printf("start tracking!\n");
      }

#if 0
      const CvRect &selection = ProcessSelection();
      if ((selection.x + selection.y + selection.width + selection.height)!=0) {
          cvRectangle( rgbImg, cvPoint(selection.x, selection.y),
              cvPoint(selection.x+selection.width, selection.y+selection.height),
              cvScalar(255, 0, 0));
      }
#endif
#endif
      //renderer->Render((uint8_t*)rgbImg->imageData, (uint8_t*)depthImg->imageData);
      renderer->Render();

      if (!renderer->Update())
        return 1;

      sleep(0);
    }

  delete renderer;


  return 0;
}

