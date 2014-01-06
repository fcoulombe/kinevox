/*
 * Copyright (C) 2011 by Francois Coulombe
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

#pragma once

#include <gcl/Config.h>

#include "renderer/Camera.h"
#include "renderer/RenderPipe.h"
#include "renderer/RenderCmd.h"
#include "renderer/RenderObject.h"
#include "renderer/ViewPort.h"
namespace GCL
{
  class Renderer
  {
  public:
      Renderer(size_t windowsHandle)
      {
		  mCamera = &Camera::DefaultCamera();
		  RenderPipe::Initialize();
		  RenderPipe::SendCommand(RenderCommand(CREATE_RENDERER, (void*)windowsHandle));
		  mViewPort.Set(0,0,Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"));
		  uint8_t *xferbuffer = new uint8_t[sizeof(ViewPort)];
		  memcpy(xferbuffer, &mViewPort, sizeof(ViewPort));
		  RenderPipe::SendCommand(RenderCommand(RENDERER_SET_VIEWPORT, (void*)xferbuffer));
      }
    ~Renderer() 
	{
		RenderPipe::SendCommand( RenderCommand(DESTROY_RENDERER));
		RenderPipe::Terminate();
	}

	void PreRender()
	{
		RenderPipe::SendCommand(RenderCommand(RENDERER_PRERENDER));
	}
	void PostRender()
	{
		RenderPipe::SendCommand(RenderCommand(RENDERER_POSTRENDER));
		RenderPipe::SendCommand(RenderCommand(SWAP_BUFFER));
	}

	void SetCamera(Camera &camera)
	{
		mCamera = &camera;
	}
    const ViewPort &GetViewPort() const { return mViewPort; }

	const std::string GetVendor() const 
	{ 
		return RenderPipe::SendCommandSyncRet( RenderCommand(RENDERER_GET_VENDOR)).GetString(); 
	}
	const std::string GetVersion() const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_VERSION)).GetString(); }
	const std::string GetRenderer() const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_RENDERER)).GetString();  }
	const std::string GetShadingLanguageVersion() const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_SHADING_LANGUAGE_VERSION)).GetString();  }
	const std::string GetGlewVersion() const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_GLEW_VERSION)).GetString();  }
	const std::vector<std::string> GetExtensions() const { return  RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_EXTENSIONS)).GetStringList();  }
	bool IsExtensionSupported(const std::string &ext) const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_IS_EXTENSION_SUPPORTED, (void*)&ext)).GetBool(); }
	bool IsGlewExtensionSupported(const std::string &ext) const { return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_IS_GLEW_EXTENSION_SUPPORTED, (void*)&ext)).GetBool(); }

	void SetOrtho()
	{
		RenderPipe::SendCommand(RenderCommand(RENDERER_SET_ORTHO));
	}
	Matrix44 GetProjection() {return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_PROJECTION)).GetMatrix(); }
	Matrix44 GetModelView() {return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_MODELVIEW)).GetMatrix();}

	static Matrix44 GetGLProjection() {return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_GL_PROJECTION)).GetMatrix(); }
	static Matrix44 GetGLModelView() {return RenderPipe::SendCommandSyncRet(RenderCommand(RENDERER_GET_GL_MODELVIEW)).GetMatrix();}
  private:
	   ViewPort mViewPort;
	   Camera *mCamera;
	   
     // IRenderer mPimpl;
  };
}
