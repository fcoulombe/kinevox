#pragma once

#include <stdint.h>

#include <libfreenect.h>

//struct freenect_device;
namespace GCL
{

uint8_t *GetKinectRgbData();
uint8_t *GetKinectDepthData();
freenect_video_format GetKinectVideoFormat();
void TiltUp();
void TiltDown();
void ResetTilt();

bool SynchronizeDrawingData();
bool InitKinect();
}
