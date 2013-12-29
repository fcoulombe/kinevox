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
#include <map>
#include "renderer/GL/GLRenderThread.h"
#include "renderer/RenderCmd.h"
#include "renderer/GL/GLRenderer.h"
#include "renderer/GL/GLGPUProgram.h"
#include "renderer/GL/GLShader.h"
#include "renderer/GL/GLTexture.h"
#include "renderer/GL/GLFrameBuffer.h"
#include "renderer/GL/GLRenderBuffer.h"
#include "renderer/GL/GLVertexBuffer.h"
#include "renderer/GL/GLPixelBufferHAL.h"
#include "renderer/RenderPipe.h"
#include "renderer/RenderObject.h"

using namespace GCL;
typedef std::map<void *, GLGPUProgram*> GPUProgramMap;
typedef std::map<void *, GLShader*> ShaderMap;
typedef std::map<void *, GLTexture*> TextureMap;
typedef std::map<void *, GLFrameBuffer*> FrameBufferMap;
typedef std::map<void *, GLRenderBuffer*> RenderBufferMap;
typedef std::map<void *, GLVertexBuffer*> VBOMap;
typedef std::map<void *, GLPixelBufferHAL*> PBOMap;

struct GLRenderData : public RenderData
{
	~GLRenderData()
	{
		mGPUProgramMap.clear();
		mShaderMap.clear();
		mTextureMap.clear();
		mFrameBufferMap.clear();
		mRenderBufferMap.clear();
		mVBOMap.clear();
		mPBOMap.clear();
	}
	GLRenderer *mRenderer;
	GPUProgramMap mGPUProgramMap;
	ShaderMap mShaderMap;
	TextureMap mTextureMap;
	FrameBufferMap mFrameBufferMap;
	RenderBufferMap mRenderBufferMap;
	VBOMap mVBOMap;
	PBOMap mPBOMap;
};

#define IS_LOGGING_RENDER_COMMAND 1
#if IS_LOGGING_RENDER_COMMAND
#define LOG_RENDER_CMD std::cout << __FUNCTION__ << std::endl;
#else
#define LOG_RENDER_CMD 
#endif
void RCSwapBuffer(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->SwapBuffer();
}
void RCCreateRenderer(RenderCommand *handle, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer = new GLRenderer((size_t)handle->mData);
}
void RCDestroyRenderer(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	delete rd.mRenderer ;
}
void RCRendererPreRender(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->PreRender();
}
void RCRendererPostRender(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->PostRender();
}
void RCRendererRender(RenderCommand *, RenderData &)
{
	LOG_RENDER_CMD
	//	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	//rd.mRenderer->Render(*(const RenderObjectList *)data->mData);
}
void RCGetVendor(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetVendor();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetVersion(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetRenderer(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetRenderer();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetShadingLanguageVersion(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetShadingLanguageVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetGlewVersion(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::string &vendor = rd.mRenderer->GetGlewVersion();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetExtensions(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const std::vector<std::string> &vendor = rd.mRenderer->GetExtensions();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCIsExtensionSupported(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	const std::string *ext = (const std::string *)data->mData;
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool vendor = rd.mRenderer->IsExtensionSupported(*ext);
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCIsGlewExtensionSupported(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	const std::string *ext = (const std::string *)data->mData;
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool vendor = rd.mRenderer->IsGlewExtensionSupported(*ext);
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCGetGLProjection(RenderCommand *, RenderData &)
{
	LOG_RENDER_CMD
	const Matrix44 vendor = GLRenderer::GetGLProjection();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetGLModelView(RenderCommand *, RenderData &)
{
	LOG_RENDER_CMD
	const Matrix44 vendor = GLRenderer::GetGLModelView();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetProjection(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		const GLRenderData &rd = static_cast<const GLRenderData&>(renderData);
		const Matrix44 &vendor = rd.mRenderer->GetProjection();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGetModelView(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		const GLRenderData &rd = static_cast<const GLRenderData&>(renderData);
		const Matrix44 &vendor = rd.mRenderer->GetModelView();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCSetViewport(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const ViewPort *viewport = (const ViewPort *)data->mData; 
	rd.mRenderer->SetViewPort(*viewport);
	uint8_t *xferbuffer = (uint8_t *)data->mData;
	delete [] xferbuffer;
}

void RCSetCamera(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const Camera *cam = (const Camera *)data->mData; 
	rd.mRenderer->SetProjection(cam);
	uint8_t *xferbuffer = (uint8_t *)data->mData;
	delete [] xferbuffer;
}
void RCSetOrtho(RenderCommand *, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mRenderer->SetOrtho();
}
void RCCreateGPUProgram(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mGPUProgramMap[data->mData] = new GLGPUProgram();
}
void RCDestroyGPUProgram(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	GPUProgramMap::iterator it = rd.mGPUProgramMap.find(data->mData);
	GCLAssert(it != rd.mGPUProgramMap.end());
	delete rd.mGPUProgramMap[data->mData];
	rd.mGPUProgramMap.erase(it);
}
void RCBindGPUProgram(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->Bind();
}
void RCAttachShader(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->AttachShader(*rd.mShaderMap[data2->mData2]);
}
void RCLinkGPUProgram(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->Link();
}
void RCIsGPUProgramValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
	GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	bool vendor = rd.mGPUProgramMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}
void RCGPUProgramSetTextureSampler(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetTextureSampler(*rd.mTextureMap[data2->mData2]);
}

void RCGPUProgramSetProjection(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const Matrix44 &proj = rd.mRenderer->GetProjection();

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetProjectionMatrix(proj);
}
void RCGPUProgramSetModelView(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const Matrix44 &modelview = rd.mRenderer->GetModelView();
	RenderCommandMatArg *data2 = static_cast<RenderCommandMatArg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetModelViewMatrix(modelview*data2->mData2);
}
void RCGPUProgramSetUniformNumber(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand3Arg *data2 = static_cast<RenderCommand3Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetUniform((const char *)data2->mData2, (long)data2->mData3);
}
void RCGPUProgramSetUniformVec2i(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommandVec2iArg *data2 = static_cast<RenderCommandVec2iArg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetUniform((const char *)data2->mData2, data2->mData3);
}
void RCGPUProgramSetUniformVec2f(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommandVec2fArg *data2 = static_cast<RenderCommandVec2fArg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->SetUniform((const char *)data2->mData2, data2->mData3);
}
void RCGPUProgramGetUniformMatrix(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	Matrix44 ret;
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->GetUniform((const char *)data2->mData2, ret);
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCGPUProgramGetUniformNumber(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	long ret;
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	rd.mGPUProgramMap[data->mData]->GetUniform((const char *)data2->mData2, ret);
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCGPUProgramGetAttributeLocation(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);

	GCLAssert(rd.mGPUProgramMap.find(data->mData) != rd.mGPUProgramMap.end());
	long ret  =	rd.mGPUProgramMap[data->mData]->GetAttributeLocation((const char *)data2->mData2);
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCGPUProgramResetDefault(RenderCommand *, RenderData &)
{
	LOG_RENDER_CMD
	GLGPUProgram::ResetDefault();
}
void RCCreateShader(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand3Arg *data3 = static_cast<RenderCommand3Arg *>(data);
	rd.mShaderMap[data->mData] = new GLShader((const char *)data3->mData2, GLShader::GetShaderType((size_t)data3->mData3));
	char *xferbuffer = (char*)data3->mData2;
	delete [] xferbuffer;
}

void RCDestroyShader(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	ShaderMap::iterator it = rd.mShaderMap.find(data->mData);
	GCLAssert(it != rd.mShaderMap.end());
	delete rd.mShaderMap[data->mData];
	rd.mShaderMap.erase(it);
}
void RCIsShaderValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool vendor = rd.mShaderMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(vendor));
}

void RCDestroyTexture(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	TextureMap::iterator it = rd.mTextureMap.find(data->mData);
	GCLAssert(it != rd.mTextureMap.end());
	delete rd.mTextureMap[data->mData];
	rd.mTextureMap.erase(it);
}
void RCCreateTexture(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);
	rd.mTextureMap[data->mData] = new GLTexture(*(const PixelBuffer *)data2->mData2);
	PixelBuffer *xferbuffer = (PixelBuffer *)data2->mData2;
	delete xferbuffer;
}
void RCBindTexture(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	rd.mTextureMap[data->mData]->Bind();
}
void RCIsTextureValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool ret = rd.mTextureMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCTextureGetWidth(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	size_t ret = rd.mTextureMap[data->mData]->GetWidth();
	RenderPipe::SendReturnMessage(new ReturnMessage((long)ret));
}
void RCTextureGetHeight(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	size_t ret = rd.mTextureMap[data->mData]->GetHeight();
	RenderPipe::SendReturnMessage(new ReturnMessage((long)ret));
}
void RCTextureGetBPP(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	size_t ret = rd.mTextureMap[data->mData]->GetBytesPerPixel();
	RenderPipe::SendReturnMessage(new ReturnMessage((long)ret));
}
void RCTextureGetTextureFromVRAM(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const uint8_t* ret = rd.mTextureMap[data->mData]->GetTextureFromVRAM();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCTextureGetPixelBufferFromVRAM(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	const uint8_t* ret = rd.mTextureMap[data->mData]->GetPixelBufferFromVRAM();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}

void RCCreateFrameBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand3Arg *data3 = static_cast<RenderCommand3Arg *>(data);
	rd.mFrameBufferMap[data->mData] = new GLFrameBuffer(*rd.mTextureMap[data3->mData2], *rd.mRenderBufferMap[data3->mData3]);
}
void RCDestroyFrameBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	FrameBufferMap::iterator it = rd.mFrameBufferMap.find(data->mData);
	GCLAssert(it != rd.mFrameBufferMap.end());
	delete rd.mFrameBufferMap[data->mData];
	rd.mFrameBufferMap.erase(it);
}
void RCBindFrameBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	rd.mFrameBufferMap[data->mData]->Bind();
}
void RCIsFrameBufferValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool ret = rd.mFrameBufferMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void FrameBufferResetDefault(RenderCommand *, RenderData &)
{
	LOG_RENDER_CMD
		GLFrameBuffer::ResetDefault();
}
void RCCreateRenderBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand3Arg *data3 = static_cast<RenderCommand3Arg *>(data);
	rd.mRenderBufferMap[data->mData] = new GLRenderBuffer((size_t)data3->mData2, (size_t)data3->mData3);
}
void RCDestroyRenderBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderBufferMap::iterator it = rd.mRenderBufferMap.find(data->mData);
	GCLAssert(it != rd.mRenderBufferMap.end());
	delete rd.mRenderBufferMap[data->mData];
	rd.mRenderBufferMap.erase(it);
}
void RCBindRenderBuffer(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);

	rd.mRenderBufferMap[data->mData]->Bind();
}
void RCIsRenderBufferValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool ret = rd.mRenderBufferMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCCreateVBO(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand5Arg *data5 = static_cast<RenderCommand5Arg *>(data);

	rd.mVBOMap[data5->mData] = GLVertexBuffer::CreateVBO((size_t)data5->mData2, data5->mData3, (size_t)data5->mData4, *(const AttribLocations *)data5->mData5);
	uint8_t *xferBuffer = (uint8_t *)data5->mData3;
	delete [] xferBuffer;
	xferBuffer = (uint8_t *)data5->mData5;
	delete [] xferBuffer;
}
void RCDestroyVBO(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	VBOMap::iterator it = rd.mVBOMap.find(data->mData);
	GCLAssert(it != rd.mVBOMap.end());
	delete rd.mVBOMap[data->mData];
	rd.mVBOMap.erase(it);
}
void RCVBORender(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand2Arg *data2 = static_cast<RenderCommand2Arg *>(data);
	rd.mVBOMap[data2->mData]->Render(GLVertexBuffer::GetRenderMode((size_t)data2->mData2));

}
void RCIsVBOValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool ret = rd.mVBOMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}

void RCCreatePBO(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mPBOMap[data->mData] = new GLPixelBufferHAL();
}
void RCDestroyPBO(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	PBOMap::iterator it = rd.mPBOMap.find(data->mData);
	GCLAssert(it != rd.mPBOMap.end());
	delete rd.mPBOMap[data->mData];
	rd.mPBOMap.erase(it);
}
void RCPBOUnBind(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mPBOMap[data->mData]->UnBind();
}
void RCPBOBind(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	rd.mPBOMap[data->mData]->Bind();
}
void RCPBOPush(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand5Arg *data5 = static_cast<RenderCommand5Arg *>(data);

	rd.mPBOMap[data->mData]->PushData((size_t)data5->mData2, (size_t)data5->mData3, (size_t)data5->mData4, (const uint8_t *)data5->mData5);
	uint8_t *xferbuffer = (uint8_t *)data5->mData5;
	delete [] xferbuffer;
}
void RCPBOPull(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	RenderCommand4Arg *data4 = static_cast<RenderCommand4Arg *>(data);
	uint8_t *ret = rd.mPBOMap[data->mData]->PullData((size_t)data4->mData2, (size_t)data4->mData3, (size_t)data4->mData4);

	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
void RCIsPBOValid(RenderCommand *data, RenderData &renderData)
{
	LOG_RENDER_CMD
		GLRenderData &rd = static_cast<GLRenderData&>(renderData);
	bool ret = rd.mPBOMap[data->mData]->IsValid();
	RenderPipe::SendReturnMessage(new ReturnMessage(ret));
}
static RenderCommandFunction GLRenderCommandMap[] =
{
	RCSwapBuffer, //0
	RCCreateRenderer, //1
	RCDestroyRenderer, //2
	RCRendererPreRender,
	RCRendererPostRender,
	RCRendererRender,
	RCGetVendor, //3
	RCGetVersion, //4
	RCGetRenderer, //5
	RCGetShadingLanguageVersion,//6
	RCGetGlewVersion, //7
	RCGetExtensions, //8
	RCIsExtensionSupported, //9
	RCIsGlewExtensionSupported,//10
	RCGetGLProjection,//11
	RCGetGLModelView,//12
	RCGetModelView,//12
	RCGetProjection,//11
	RCSetViewport,//13
	RCSetCamera,//14
	RCSetOrtho,//15
	RCCreateGPUProgram,//16
	RCDestroyGPUProgram,//17
	RCBindGPUProgram,//18
	RCAttachShader,//19
	RCLinkGPUProgram,//20
	RCIsGPUProgramValid,//21
	RCGPUProgramSetTextureSampler,//22
	RCGPUProgramSetProjection,//23
	RCGPUProgramSetModelView,//24
	RCGPUProgramSetUniformNumber,
	RCGPUProgramSetUniformVec2i,
	RCGPUProgramSetUniformVec2f,
	RCGPUProgramGetUniformMatrix,//25
	RCGPUProgramGetUniformNumber,//26
	RCGPUProgramGetAttributeLocation,//27
	RCGPUProgramResetDefault, //28
	RCCreateShader,//29
	RCDestroyShader,//30
	RCIsShaderValid,
	RCDestroyTexture,
	RCCreateTexture,
	RCBindTexture,
	RCIsTextureValid,
	RCTextureGetWidth,
	RCTextureGetHeight,
	RCTextureGetBPP,
	RCTextureGetTextureFromVRAM,
	RCTextureGetPixelBufferFromVRAM,
	RCCreateFrameBuffer,
	RCDestroyFrameBuffer,
	RCBindFrameBuffer,
	RCIsFrameBufferValid,
	FrameBufferResetDefault,
	RCCreateRenderBuffer,
	RCDestroyRenderBuffer,
	RCBindRenderBuffer,
	RCIsRenderBufferValid,
	RCCreateVBO,
	RCDestroyVBO,
	RCVBORender,
	RCIsVBOValid,
	RCCreatePBO,
	RCDestroyPBO,
	RCPBOUnBind,
	RCPBOBind,
	RCPBOPush,
	RCPBOPull,
	RCIsPBOValid
};
GCL::GLRenderThread::GLRenderThread()
{
	mRenderCommandMap = GLRenderCommandMap;
	mRenderData = new GLRenderData();
}

GCL::GLRenderThread::~GLRenderThread()
{
	mIsRunning = false;
	mRunMutex.Notify();
	if (IsJoinable())
		Join();
	delete mRenderData;
}
