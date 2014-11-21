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


#include "renderer/GLES/GLESRenderer.h"

#include <sstream>

#include <3rdparty/OpenGL.h>
#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/Log.h>
#include <gcl/StringUtil.h>

#include "renderer/Camera.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/Text2D.h"
#include "renderer/GLES/GLESGPUProgram.h"
#include "renderer/GLES/GLESRenderUtils.h"
#include "renderer/GLES/GLESShader.h"
#include "renderer/GLES/GLESVertexBuffer.h"

using namespace GCL;

void GLESRenderer::Init3DState()
{

	glErrorCheck();
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); glErrorCheck();
	glClearDepth(1.0); glErrorCheck();
	glDepthMask(GL_TRUE); glErrorCheck();
	glDepthFunc(GL_LESS); glErrorCheck();
	glEnable(GL_DEPTH_TEST); glErrorCheck();
	glEnable(GL_BLEND); glErrorCheck();
	glEnable(GL_CULL_FACE);glErrorCheck();
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
	//glActiveTexture(GL_TEXTURE0);glErrorCheck();
	
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); glErrorCheck();
}

void eglErrorCheck()
{
	/*
		eglGetError returns the last error that has happened using egl,
		not the status of the last called function. The user has to
		check after every single egl call or at least once every frame.
	*/
	EGLint iErr = eglGetError();
	std::string errorString = "";
	switch (iErr)
	{
	case EGL_SUCCESS:
		return;
	//case EGL_NO_CONTEXT:
	//	errorString = "is returned if creation of the context fails.";
	//	break;
	case EGL_BAD_MATCH:
		errorString = "is generated if the current rendering API is EGL_NONE (this can only arise in an EGL implementation which does not support OpenGL ES, prior to the first call to eglBindAPI).";
		errorString += "\nor\n";
		errorString += "is generated if the server context state for share_context exists in an address space which cannot be shared with the newly created context, if share_context was created on a different display than the one referenced by config, or if the contexts are otherwise incompatible.";
		break;
	case EGL_BAD_DISPLAY:
		errorString = "is generated if display is not an EGL display connection.";
		break;
	case EGL_NOT_INITIALIZED:
		errorString = "is generated if display has not been initialized.";
		break;
	case EGL_BAD_CONFIG:
		errorString = "is generated if config is not an EGL frame buffer configuration, or does not support the current rendering API. This includes requesting creation of an OpenGL ES 1.x context when the EGL_RENDERABLE_TYPE attribute of config does not contain EGL_OPENGL_ES_BIT, or creation of an OpenGL ES 2.x context when the attribute does not contain EGL_OPENGL_ES2_BIT.";
		break;
	case EGL_BAD_CONTEXT:
		errorString = "is generated if share_context is not an EGL rendering context of the same client API type as the newly created context and is not EGL_NO_CONTEXT.";
		break;
	case EGL_BAD_ATTRIBUTE:
		errorString = "is generated if attrib_list contains an invalid context attribute or if an attribute is not recognized or out of range. Note that attribute EGL_CONTEXT_CLIENT_VERSION is only valid when the current rendering API is EGL_OPENGL_ES_API.";
		break;
	case EGL_BAD_ALLOC:
		errorString = "is generated if there are not enough resources to allocate the new context.";
		break;
	default:
		errorString = "Not SureWhat Happened";
	}
	
	GCLAssertMsg(false, errorString.c_str());

}

void GLESRenderer::InitWin(size_t windowsHandle)
{
#ifdef OS_WIN32
    HDC hDC = GetDC((HWND)windowsHandle);
    GCLAssertMsg(hDC, "Failed to create the device context");
    mEglDisplay = eglGetDisplay(hDC);
    if(mEglDisplay == EGL_NO_DISPLAY)
        mEglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
#else
    (void)windowsHandle;
#endif
}
void GLESRenderer::InitLinux(size_t windowsHandle)
{
#ifdef OS_LINUX
	Display*			x11Display	= mDisplay = (Display*)windowsHandle;
	long x11Screen  = XDefaultScreen( x11Display );

	// Gets the window parameters
	Window	sRootWindow = RootWindow(x11Display, x11Screen);
	int i32Depth = DefaultDepth(x11Display, x11Screen);
	x11Visual = new XVisualInfo;
	XMatchVisualInfo( x11Display, x11Screen, i32Depth, TrueColor, x11Visual);
	GCLAssertMsg (x11Visual, "Unable to acquire visual");

    mCmap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );
    XSetWindowAttributes	sWA;

    sWA.colormap = mCmap;

    // Add to these for handling other events
    sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
    unsigned int ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;
	int i32Width  = (int)mViewPort.GetWidth()  < XDisplayWidth(x11Display, x11Screen) ? (int)mViewPort.GetWidth() : XDisplayWidth(x11Display, x11Screen);
	int i32Height = (int)mViewPort.GetHeight() < XDisplayHeight(x11Display,x11Screen) ? (int)mViewPort.GetHeight(): XDisplayHeight(x11Display,x11Screen);

	// Creates the X11 window
    mWin = XCreateWindow( x11Display, RootWindow(x11Display, x11Screen), 0, 0, i32Width, i32Height,
								 0, CopyFromParent, InputOutput, CopyFromParent, ui32Mask, &sWA);
	XMapWindow(x11Display, mWin);
	XFlush(x11Display);


	mEglDisplay = eglGetDisplay((EGLNativeDisplayType)x11Display);

	mEglWindow = (EGLNativeWindowType)mWin;
#else
	(void)windowsHandle;
#endif

}

GLESRenderer::GLESRenderer(size_t windowsHandle)
	:   mModelView(true),
		mFov(45.0),
	mAspect(Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH") / Real(Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"))),
	mNear(0.1),
	mFar(100.0)
{
	RenderPipe::SendCommand([this, windowsHandle](){
		KLog("Start init renderer");
		mProjection.SetPerspective(mFov,mAspect,mNear,mFar);
#ifdef OS_ANDROID
	InitWin(windowsHandle);
	InitLinux(windowsHandle);
	mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint iMajorVersion, iMinorVersion;
    EGLBoolean ret = eglInitialize(mEglDisplay, &iMajorVersion, &iMinorVersion);
    GCLAssertMsg(ret, "eglInitialize() failed.");
    //ret = eglBindAPI(EGL_OPENGL_ES_API);


#	if defined(ES2)
#		ifdef OS_ANDROID
    const EGLint pi32ConfigAttribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
#		else
	EGLint pi32ConfigAttribs[5];
	pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
	pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;
	pi32ConfigAttribs[4] = EGL_NONE;
#		endif

#	elif defined(ES3)
#		error "TBD"
#	endif
    EGLint iConfigs;
    ret = eglChooseConfig(mEglDisplay, pi32ConfigAttribs, &mEglConfig, 1, &iConfigs) || (iConfigs != 1);
    GCLAssertMsg(ret, "eglChooseConfig() failed.");
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    EGLint format;
    ret = eglGetConfigAttrib(mEglDisplay, mEglConfig, EGL_NATIVE_VISUAL_ID, &format);
    GCLAssert(ret);
    ANativeWindow_setBuffersGeometry((ANativeWindow*)windowsHandle, 0, 0, format);

    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, (ANativeWindow*)windowsHandle, NULL);
    if (mEglSurface == EGL_NO_SURFACE)
    {    
        eglGetError(); // Clear error
        mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, (EGLNativeWindowType)NULL, NULL);
	}


#	if defined(ES2)
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
#	else
#		error "TBD"
#	endif
	mEglContext = eglCreateContext(mEglDisplay, mEglConfig, NULL, ai32ContextAttribs);
	eglErrorCheck();
	auto mkRet = eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);
	GCLAssert(mkRet != EGL_FALSE);
	eglErrorCheck();

	int i32Values;
	ret = eglGetConfigAttrib(mEglDisplay, mEglConfig, EGL_DEPTH_SIZE , &i32Values);
	GCLAssert(ret);
#endif
	Init3DState();
	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	KLog("Version: %s", mVersion.c_str());
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	KLog("Vendor: %s", mVendor.c_str());
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
	KLog("Render: %s", mRenderer.c_str());
	const char *ver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);glErrorCheck();
	mShadingLanguageVersion = std::string(ver);
	KLog("Shading Language: %s", mShadingLanguageVersion.c_str());
	char delim = ' ';
	std::string extString((const char *) glGetString(GL_EXTENSIONS));
	StringUtil::Explode(extString,mExtensions, delim); glErrorCheck();

#if ENABLE_GLEW
	GLenum err = glewInit();
	GCLAssertMsg(GLEW_OK == err, (const char *)glewGetErrorString(err));
	mGlewVersion = std::string((const char*)glewGetString(GLEW_VERSION));
#elif ENABLE_EGLEW
    mGlewVersion  = std::string("eglew");
    eglewInit();
#else
	mGlewVersion  = std::string("Unused");
#endif
	});

}
GLESRenderer::~GLESRenderer()
{
	RenderPipe::SendCommandSync([&](){
	mExtensions.clear();
	eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ;
	eglTerminate(mEglDisplay);

#ifdef OS_WIN32
	ReleaseDC( mhWnd, mhDC );
#elif defined(OS_LINUX)
	if (mWin)
		XDestroyWindow(mDisplay, mWin);
    if (mCmap)
    	XFreeColormap( mDisplay, mCmap);

    delete x11Visual;

#endif
	});
}

bool GLESRenderer::Update()
{
	return true;
}


void GLESRenderer::PreRender()
{
	RenderPipe::SendCommand([&](){
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
	});
}
void GLESRenderer::PostRender()
{
	RenderPipe::SendCommand([&](){
		eglSwapBuffers(mEglDisplay, mEglSurface);
	});
}


void GLESRenderer::RenderState::SetTextureEnabled(bool isEnabled)
{
	RenderPipe::SendCommand([&](){
	if (isEnabled)
	{
		glEnable(GL_TEXTURE_2D); glErrorCheck();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
		glDisable(GL_TEXTURE_2D); glErrorCheck();
	}
	});
}


Matrix44 GLESRenderer::GetGLProjection()
{
    Matrix44 projectionMatrixd;
    GCLAssert(false && "UNSUPPORTED");
	return projectionMatrixd;
}
Matrix44 GLESRenderer::GetGLModelView()
{
    Matrix44 modelViewMatrixd;
    GCLAssert(false && "UNSUPPORTED");
	return modelViewMatrixd;
}

void GLESRenderer::SwapBuffer()
{

	RenderPipe::SendCommand([&](){
	eglSwapBuffers(mEglDisplay, mEglSurface);
	});
}

void GCL::GLESRenderer::SetProjection( const Camera &camera )
{
	RenderPipe::SendCommand([&](){
	mProjection.SetPerspective(camera.GetFov(),camera.GetAspectRatio(),camera.GetNear(),camera.GetFar());
	mModelView= Inverse(camera.GetTransform());
	});
}

void GCL::GLESRenderer::SetIsDepthTesting( bool isDepthTesting /*= true*/ )
{
    RenderPipe::SendCommand([=](){
        if (isDepthTesting)
        {
            glEnable(GL_DEPTH_TEST); glErrorCheck();
        }
        else
        {
            glDisable(GL_DEPTH_TEST); glErrorCheck();
        }
    });
}

void GCL::GLESRenderer::GetScreenSize(Point2<size_t> &screenSize) const
{
	RenderPipe::SendCommandSync([&](){
#if defined(OS_ANDROID)
		EGLint w, h;//, dummy, format;
	    GLboolean ret;
	    ret = eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &w);
	    GCLAssert(ret);
	    ret = eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &h);
	    screenSize = Point2<size_t>((size_t)(w), (size_t)(h));
#elif defined(OS_WIN32)
	RECT rect;
	::GetWindowRect(mHwnd,&rect);
	screenSize = Point2<size_t>((size_t)(rect.right-rect.left), (size_t)(rect.top-rect.bottom));
#elif defined(OS_LINUX)
	XWindowAttributes xwa;
	XGetWindowAttributes(mDisplay, mWin, &xwa);
	screenSize = Point2<size_t>(size_t(xwa.width), size_t(xwa.height));
#endif
	});
}
