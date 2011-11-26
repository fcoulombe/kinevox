
#include "Renderer/CVRenderer.h"

#include <highgui.h>


using namespace GCL;




OpenCVRenderer::OpenCVRenderer()
{
	rgbImg =cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
	bgrImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
	depthImg = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 1);

	cvNamedWindow("rgb",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("depth",CV_WINDOW_AUTOSIZE);
	//moveWindow("rgb", 0, 0);
	//moveWindow("depth", 640, 0);

}
OpenCVRenderer::~OpenCVRenderer()
{
	cvDestroyWindow("rgb");
	cvDestroyWindow("depth");
	cvReleaseImage(&rgbImg);
	cvReleaseImage(&depthImg);
}
bool OpenCVRenderer::Update()
{
	if( (char)27==cv::waitKey(1) )
		return false;
	return true;

}
void OpenCVRenderer::Render(uint8_t *rgb_front, uint8_t *depth_front)
{
	cvSetData(rgbImg, rgb_front, 640*3);
	cvCvtColor(rgbImg,bgrImg, CV_RGB2BGR );

	cvSetData(depthImg, depth_front, 640*3);
	cvShowImage("rgb", bgrImg);
	cvShowImage("depth", depthImg);

}
