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

using namespace GCL;
struct GLRenderData : public RenderData
{
	GLRenderer *mRenderer;
};

void RCSwapBuffer(void *, RenderData &renderData);
void RCCreateRenderer(void *, RenderData &renderData);
void RCCreateVertexBuffer(void *, RenderData &renderData);
void RCCreateTexture(void *, RenderData &renderData);
void RCCreateShader(void *, RenderData &renderData);
void RCCreateVAO(void *, RenderData &renderData);

void RCSwapBuffer(void *, RenderData &renderData)
{
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->SwapBuffer();
}
void RCCreateRenderer(void *handle, RenderData &renderData)
{
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer = new GLRenderer((size_t)handle);
}
void RCCreateVertexBuffer(void *, RenderData &)
{
	//create glvertex buffer
	//return id
}
void RCCreateTexture(void *, RenderData &)
{
}
void RCCreateShader(void *, RenderData &)
{
}
void RCCreateVAO(void *, RenderData &)
{
}

static RenderCommandFunction GLRenderCommandMap[] =
{
	RCSwapBuffer,
	RCCreateRenderer,
	RCCreateVertexBuffer,
	RCCreateTexture,
	RCCreateShader,
	RCCreateVAO
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
