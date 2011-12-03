#pragma once
#include <cstdlib>

namespace GCL
{
class FrameBuffer;
class RenderBuffer;
class Texture;
class GCLRenderTarget
{
public:
	GCLRenderTarget(size_t width, size_t height);
	~GCLRenderTarget();

	void Bind();

	void Save(const char *filename);
private:
	FrameBuffer *mFrameBuffer;
	RenderBuffer *mDepthBuffer;
	Texture *mColorBuffer;
};

}
