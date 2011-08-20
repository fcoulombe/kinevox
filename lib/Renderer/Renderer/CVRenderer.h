#pragma once

#include "Renderer.h"
#include <cv.h>

namespace GCL
{
class OpenCVRenderer : public Renderer
{
public:
	OpenCVRenderer();
	~OpenCVRenderer();
	bool Update();
	void Render(uint8_t *rgb_front, uint8_t *depth_front);
	IplImage* rgbImg;
	IplImage* bgrImg ;
	IplImage* depthImg;
};
}

