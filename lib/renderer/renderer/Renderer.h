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

#include "renderer/RenderPipe.h"
#include "renderer/RenderCmd.h"
#include "renderer/ViewPort.h"
namespace GCL
{
  class Renderer
  {
  public:
      Renderer(size_t windowsHandle)
      {
		  RenderPipe::Initialize();
		  RenderPipe::SendCommand(new RenderCommand(CREATE_RENDERER, (void*)windowsHandle));
		  mViewPort.Set(0,0,Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"));
		  RenderPipe::SendCommand(new RenderCommand(SET_VIEWPORT, (void*)&mViewPort));
      }
    ~Renderer() 
	{
		RenderPipe::SendCommand(new RenderCommand(DESTROY_RENDERER));
		RenderPipe::Terminate();
	}

    const ViewPort &GetViewPort() const { return mViewPort; }

	const std::string GetVendor() const 
	{ 
		return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_VENDOR)).GetString(); 
	}
	const std::string GetVersion() const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_VERSION)).GetString(); }
	const std::string GetRenderer() const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_RENDERER)).GetString();  }
	const std::string GetShadingLanguageVersion() const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_SHADING_LANGUAGE_VERSION)).GetString();  }
	const std::string GetGlewVersion() const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_GLEW_VERSION)).GetString();  }
	const std::vector<std::string> GetExtensions() const { return  RenderPipe::SendCommandSyncRet(new RenderCommand(GET_EXTENSIONS)).GetStringList();  }
	bool IsExtensionSupported(const std::string &ext) const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_IS_EXTENSION_SUPPORTED, (void*)&ext)).GetBool(); }
	bool IsGlewExtensionSupported(const std::string &ext) const { return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_IS_GLEW_EXTENSION_SUPPORTED, (void*)&ext)).GetBool(); }

	static Matrix44 GetGLProjection() {return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_GL_PROJECTION)).GetMatrix(); }
	static Matrix44 GetGLModelView() {return RenderPipe::SendCommandSyncRet(new RenderCommand(GET_GL_MODELVIEW)).GetMatrix();}
  private:
	   ViewPort mViewPort;
	   
     // IRenderer mPimpl;
  };
}
