#include "applayer/GCLRenderTarget.h"
#include <renderer/FrameBuffer.h>
#include <renderer/RenderBuffer.h>
#include <renderer/Texture.h>


using namespace GCL;

GCLRenderTarget::GCLRenderTarget(size_t width, size_t height)
{
	mDepthBuffer = new RenderBuffer(width, height),
	mColorBuffer = new Texture(width, height, 4);
	mFrameBuffer = new FrameBuffer(*mColorBuffer, *mDepthBuffer);
	Bind();
}

GCLRenderTarget::~GCLRenderTarget()
{
	delete mFrameBuffer;
	delete mColorBuffer;
	delete mDepthBuffer;
}


void GCLRenderTarget::Bind()
{
	mDepthBuffer->Bind();
	mColorBuffer->Bind();
	mFrameBuffer->Bind();
}


void GCLRenderTarget::Save(const char *filename)
{
	mColorBuffer->Save(filename);
}
