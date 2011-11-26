#pragma once

#include <cstdlib>
#include <stdint.h>

#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"
#include "Renderer/ViewPort.h"


namespace GCL
{

class GLRenderer : public Renderer
{
public:
	GLRenderer();
	~GLRenderer();
	bool Update();
	virtual void Render(const RenderObjectList &renderObjectList);
	void Render(uint8_t *rgb_front, uint8_t *depth_front);
	void RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth);


	void SetCamera(const Camera &camera) { mCamera = &camera; }

	const ViewPort &GetViewPort() const { return mViewPort; }

	uint32_t gl_depth_tex;
	uint32_t gl_rgb_tex;

private:
	void Init3DState();
	void Init2DState();

	const Camera *mCamera;
	ViewPort mViewPort;

};
}
