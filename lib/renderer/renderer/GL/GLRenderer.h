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

#include <algorithm>
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>

#include <3rdparty/OpenGL.h>
#include <gcl/Matrix44.h>
#include "renderer/ViewPort.h"
#include "renderer/RenderPipe.h"

#if defined(OS_LINUX)
#include <GL/glx.h>
#endif
namespace GCL
{
    class Text2D;
	class Camera;
    
    typedef std::vector<Text2D*> Text2DList;
class GLGPUProgram;
class GLRenderer
{
public:
	GLRenderer(size_t windowsHandle);
	~GLRenderer();
	bool Update();
	void PreRender();
	void PostRender();

	struct RenderState
	{
		void SetTextureEnabled(bool isEnabled);
	};
	RenderState mCurrentRenderState;

	void GetVendor(std::string &vendor) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetVendorUnsafe(vendor);
		});
	}
	void GetVersion(std::string &version) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetVersionUnsafe(version);
		});
	}
	void GetRenderer(std::string &version) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetRendererUnsafe(version);
		});
	}
	void GetShadingLanguageVersion(std::string &shadingLanguageVersion) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetShadingLanguageVersionUnsafe(shadingLanguageVersion);
		});
	}
	void GetGlewVersion(std::string &glewVersion) const
	{ 
		RenderPipe::SendCommandSync([&](){
			GetGlewVersionUnsafe(glewVersion);
		});
	}
	void GetExtensions(std::vector<std::string> &extensions) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetExtensionsUnsafe(extensions);
		});
	}

	bool IsExtensionSupported(const std::string &ext) const
	{
		bool ret;
		RenderPipe::SendCommandSync([&](){
			ret = IsExtensionSupportedUnsafe(ext);
		});
		return ret;
	}
	bool IsGlewExtensionSupported(const std::string &ext) const
	{
		bool ret;
		RenderPipe::SendCommandSync([&](){
		ret = (glewGetExtension(ext.c_str())) ? true : false;
		});
		return ret;
	}


	static Matrix44 GetGLProjection();
	static Matrix44 GetGLModelView();
	void GetProjection(Matrix44 &projection) const 
	{ 
		RenderPipe::SendCommandSync([&](){
			GetProjectionUnsafe(projection);
		}); 
	}
	void GetModelView(Matrix44 &modelView) const
	{ 
		RenderPipe::SendCommandSync([&](){
			GetModelViewUnsafe(modelView);
		}); 
	}
	void SwapBuffer();
	void SetViewPort(const ViewPort &viewport)
	{
		RenderPipe::SendCommand([=](){
			SetViewPortUnsafe(viewport);
		});
	}
	void SetProjection(const Matrix44 &projection)
	{
		RenderPipe::SendCommand([=](){
			SetProjectionUnsafe(projection);
		});
	}
	void SetProjection(const Camera &camera);
	void SetOrtho()
	{
		RenderPipe::SendCommand([&](){
			SetOrthoUnsafe();
		});
	}
	void SetModelView(const Matrix44 &modelView)
	{
		RenderPipe::SendCommand([=](){
			SetModelViewUnsafe(modelView);
		});
	}

    void SetIsDepthTesting(bool isDepthTesting = true);

    void GetScreenSize(Point2<size_t> &screenSize) const;
private:
	void Init3DState();
   
	std::string mVendor,
				mVersion,
				mRenderer,
				mShadingLanguageVersion,
				mGlewVersion;

	std::vector<std::string> mExtensions;
	ViewPort mViewPort;
	Matrix44 mProjection;
	Matrix44 mModelView;
	Real mFov, mAspect, mNear, mFar;

	void InitWGL(size_t windowsHandle);
	void InitGLX(size_t windowsHandle);
#if defined(OS_WIN32)

	HWND mhWnd;
	HDC mhDC;
	HGLRC mhRC;
#elif defined(OS_LINUX)
	Display *mDisplay;
	Window mWin;
	GLXContext mCtx;
	//Colormap mCmap;
#endif
	void SetOrthoUnsafe()
	{
		mProjection.SetOrtho(0.0,  (Real)mViewPort.GetWidth(), (Real)mViewPort.GetHeight(),0.0, -1.0, 1.0);
		mModelView.SetIdentity();
	}
	void SetModelViewUnsafe(const Matrix44 &modelView)
	{
		mModelView = modelView;
	}
	void SetViewPortUnsafe(const ViewPort &viewport)
	{
		mViewPort = viewport;
		glViewport(viewport.GetX(),viewport.GetY(),viewport.GetWidth(), viewport.GetHeight());glErrorCheck();
	}
	void SetProjectionUnsafe(const Matrix44 &projection)
	{
		mProjection = projection;
	}
	void GetProjectionUnsafe(Matrix44 &projection) const { projection = mProjection; }
	void GetModelViewUnsafe(Matrix44 &modelView) const { modelView = mModelView; }
	void GetVendorUnsafe(std::string &vendor) const { vendor = mVendor; }
	void GetVersionUnsafe(std::string &version) const { version = mVersion; }
	void GetRendererUnsafe(std::string &renderer) const { renderer = mRenderer; }
	void GetShadingLanguageVersionUnsafe(std::string &shadingLanguageVersion) const { shadingLanguageVersion = mShadingLanguageVersion; }
	void GetGlewVersionUnsafe(std::string &glewVersion) const { glewVersion = mGlewVersion; }
	void GetExtensionsUnsafe(std::vector<std::string> &extensions) const { extensions = mExtensions; }

	bool IsExtensionSupportedUnsafe(const std::string &ext) const
	{
		std::vector<std::string>::const_iterator b = mExtensions.begin();
		std::vector<std::string>::const_iterator e = mExtensions.end();
		std::vector<std::string>::const_iterator r =  std::find(b, e, ext);

		bool res = r != mExtensions.end();
		return res;
	}
	bool IsGlewExtensionSupportedUnsafe(const std::string &ext) const
	{
		bool ret;
		RenderPipe::SendCommand([&](){
			ret = (glewGetExtension(ext.c_str())) ? true : false;
		});
		return ret;
	}
};
}
