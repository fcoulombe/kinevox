#pragma once

#include <cstdlib>
#include <stdint.h>

#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"


namespace GCL
{

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	bool Update();
	void Render();
	void Render(uint8_t *rgb_front, uint8_t *depth_front);
	void RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth);


	Camera &GetCamera() { return mCamera; }

	uint32_t gl_depth_tex;
	uint32_t gl_rgb_tex;

private:
	void Init3DState();
	void Init2DState();

	Camera mCamera;

};
}
