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
#include "rendererconf.h"
#include GFXAPI_Renderer_H
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

		  mPimpl = new IRenderer(windowsHandle);
		  mViewPort.Set(0,0,Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"));
		  mPimpl->SetViewPort(mViewPort);
      }
    ~Renderer() 
	{
		delete mPimpl;
	}

	void PreRender()
	{
		mPimpl->PreRender();
	}
	void PostRender()
	{
		mPimpl->PostRender();
		mPimpl->SwapBuffer();	
	}

	void SetCamera(Camera &camera)
	{
		mCamera = &camera;
		mPimpl->SetProjection(camera);
	}
	void SetViewPort(const ViewPort &viewport)
	{
		mViewPort = viewport;
		mPimpl->SetViewPort(mViewPort);
	}
    const ViewPort &GetViewPort() const { return mViewPort; }
    void GetScreenSize(Point2<size_t> &screenSize) const { mPimpl->GetScreenSize(screenSize); }

	void GetVendor(std::string &vendor) const 
	{ 
		mPimpl->GetVendor(vendor);
	}
	void GetVersion(std::string &version) const { mPimpl->GetVersion(version); }
	void GetRenderer(std::string &renderer) const { mPimpl->GetRenderer(renderer); }
	void GetShadingLanguageVersion(std::string  &shadingLanguageVersion) const { mPimpl->GetShadingLanguageVersion(shadingLanguageVersion); }
	void GetGlewVersion(std::string  &glewVersion) const { mPimpl->GetGlewVersion(glewVersion); }
	void GetExtensions(std::vector<std::string>  &extensions) const { mPimpl->GetExtensions(extensions); }
	bool IsExtensionSupported(const std::string &ext) const { return mPimpl->IsExtensionSupported(ext); }
	bool IsGlewExtensionSupported(const std::string &ext) const { return mPimpl->IsGlewExtensionSupported(ext); }

	void SetOrtho()
	{
		mPimpl->SetOrtho();
	}
    void SetIsDepthTesting(bool isDepthTesting = true)
    {
        mPimpl->SetIsDepthTesting(isDepthTesting);
    }
	void GetProjection(Matrix44  &projection) {mPimpl->GetProjection(projection); }
	void GetModelView(Matrix44  &modelView) {mPimpl->GetModelView(modelView); }

	static Matrix44 GetGLProjection() {return IRenderer::GetGLProjection(); }
	static Matrix44 GetGLModelView() {return IRenderer::GetGLModelView(); }
  private:
	  ViewPort mViewPort;
	  Camera *mCamera;
	  IRenderer *mPimpl;
  };
}
