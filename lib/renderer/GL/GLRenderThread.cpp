/*
 * Copyright (C) 2013 by Francois Coulombe
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

#include "renderer/GL/GLRenderThread.h"
#include "renderer/RenderCmd.h"
#include "renderer/GL/GLRenderer.h"
#include "renderer/RenderPipe.h"

using namespace GCL;
struct GLRenderData : public RenderData
{
	GLRenderer *mRenderer;
};

#define IS_LOGGING_RENDER_COMMAND 1
#if IS_LOGGING_RENDER_COMMAND
#define LOG_RENDER_CMD std::cout << __FUNCTION__ << std::endl;
#else
#define LOG_RENDER_CMD 
#endif
void RCSwapBuffer(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->SwapBuffer();
}
void RCCreateRenderer(void *handle, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer = new GLRenderer((size_t)handle);
}
void RCCreateVertexBuffer(void *, RenderData &)
{
	LOG_RENDER_CMD
	//create glvertex buffer
	//return id
}
void RCCreateTexture(void *, RenderData &)
{
	LOG_RENDER_CMD
}
void RCCreateShader(void *, RenderData &)
{
	LOG_RENDER_CMD
}
void RCCreateVAO(void *, RenderData &)
{
	LOG_RENDER_CMD
}

void RCGetVendor(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetVendor();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetVersion(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetRenderer(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetRenderer();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetShadingLanguageVersion(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetShadingLanguageVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetGlewVersion(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetGlewVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetExtensions(void *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::vector<std::string> &vendor = rd.mRenderer->GetExtensions();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCIsExtensionSupported(void *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	const std::string *ext = (const std::string *)data;
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool vendor = rd.mRenderer->IsExtensionSupported(*ext);
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCIsGlewExtensionSupported(void *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	const std::string *ext = (const std::string *)data;
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool vendor = rd.mRenderer->IsGlewExtensionSupported(*ext);
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetGLProjection(void *, RenderData &)
{
	LOG_RENDER_CMD
	//const GLRenderData &rd = static_cast<const GLRenderData&>(renderData);
	const Matrix44 &vendor = GLRenderer::GetGLProjection();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetGLModelView(void *, RenderData &)
{
	LOG_RENDER_CMD
	const Matrix44 &vendor = GLRenderer::GetGLModelView();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCSetViewport(void *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const ViewPort *viewport = (const ViewPort *)data; 
	rd.mRenderer->SetViewPort(*viewport);
}
void RCSetProjection(void *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const Matrix44 *viewport = (const Matrix44 *)data; 
	rd.mRenderer->SetProjection(*viewport);
}
void RCSetModelView(void *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const Matrix44 *viewport = (const Matrix44 *)data; 
	rd.mRenderer->SetModelView(*viewport);
}
static RenderCommandFunction GLRenderCommandMap[] =
{
	RCSwapBuffer,
	RCCreateRenderer,
	RCCreateVertexBuffer,
	RCCreateTexture,
	RCCreateShader,
	RCCreateVAO,
	RCGetVendor,
	RCGetVersion,
	RCGetRenderer,
	RCGetShadingLanguageVersion,
	RCGetGlewVersion,
	RCGetExtensions,
	RCIsExtensionSupported,
	RCIsGlewExtensionSupported,
	RCGetGLProjection,
	RCGetGLModelView,
	RCSetViewport,
	RCSetProjection,
	RCSetModelView
};
GCL::GLRenderThread::GLRenderThread()
{
	mRenderCommandMap = GLRenderCommandMap;
	mRenderData = new GLRenderData();
}

GCL::GLRenderThread::~GLRenderThread()
{
	delete mRenderData;
}
