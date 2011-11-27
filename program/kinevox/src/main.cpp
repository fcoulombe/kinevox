
#include <unistd.h>


#include <applayer/GCLApplication.h>
#include <input/Input.h>
#include <kinect/KinectDriver.h>
#include <renderer/Camera.h>
#include <renderer/OpenCV.h>

//#include "Voxel/Grid.h"

using namespace GCL;

int use_nested_cascade = 0;


int main(int argc, char **argv)
{
  /*if (!InitKinect())
		return -1;*/

	GCLApplication::Initialize();
	Camera myCamera;
	GCLApplication::SetViewportCamera(myCamera);



  IplImage* rgbImg;
  IplImage* depthImg;
  rgbImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
  depthImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);

  //	Grid grid(10,10,10);

  //	renderer->RegisterRenderObject(&grid);
  while (true)
    {

	  GCLApplication::Update();
      if (!SynchronizeDrawingData())
        continue;


      cvSetData(rgbImg, GetKinectRgbData(), 640*3);
      cvSetData(depthImg, GetKinectDepthData(), 640*3);

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
    	  myCamera.TiltDown();



      if(Input::IsKeyUp(306)) { //left ctrl
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

      //renderer->Render((uint8_t*)rgbImg->imageData, (uint8_t*)depthImg->imageData);

		GCLApplication::Render();
		usleep(100);
		std::cout.flush();
    }

  return 0;
}

