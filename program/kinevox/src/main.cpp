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
#if ENABLE_FREENECT

#include <gcl/Time.h> 
#include <input/Input.h>
#include <kinect/KinectDriver.h>
#include <renderer/Camera.h>
#include <renderer/OpenCV.h>

//#include "Voxel/Grid.h"

using namespace GCL;

int use_nested_cascade = 0;


int main(int /*argc*/, char ** /*argv*/)
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
		Time::SleepMs(100);
		std::cout.flush();
    }

  return 0;
}
#else

int main(int /*argc*/, char ** /*argv*/)
{

    return 0;
}
#endif
