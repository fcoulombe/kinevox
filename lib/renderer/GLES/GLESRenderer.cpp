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
    glViewport(0,0,(GLsizei)mViewPort.GetWidth(),(GLsizei)mViewPort.GetHeight()); 
	glErrorCheck();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
	glClearDepth(1.0); glErrorCheck();
	
	glDepthMask(GL_TRUE); glErrorCheck();
	
	glDepthFunc(GL_LESS); glErrorCheck();
	
//	glEnable(GL_DEPTH_TEST); glErrorCheck();
	
	//glDisable(GL_BLEND); glErrorCheck();

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
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
    eglDisplay = eglGetDisplay(hDC);
    if(eglDisplay == EGL_NO_DISPLAY)
        eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
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


	eglDisplay = eglGetDisplay((EGLNativeDisplayType)x11Display);

	eglWindow = (EGLNativeWindowType)mWin;
#else
	(void)windowsHandle;
#endif

}
GLESRenderer::GLESRenderer(size_t windowsHandle)
	:   mModelView(true),
		mFov(45.0),
	mAspect(640.0/480.0),
	mNear(0.1),
	mFar(100.0)
{
	InitWin(windowsHandle);
	InitLinux(windowsHandle);

    EGLint iMajorVersion, iMinorVersion;
    EGLBoolean ret = eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion);
    GCLAssertMsg(ret, "eglInitialize() failed.");
    ret = eglBindAPI(EGL_OPENGL_ES_API); 


#if defined(ES2)
   /* const EGLint pi32ConfigAttribs[] =
    {
        EGL_LEVEL,				0,
        EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
        EGL_NATIVE_RENDERABLE,	EGL_FALSE,
        EGL_DEPTH_SIZE,			EGL_DONT_CARE,
        EGL_NONE
    };*/
	EGLint pi32ConfigAttribs[5];
	pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
	pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;
	pi32ConfigAttribs[4] = EGL_NONE;

#elif defined(ES3)
#error "TBD"
#endif
    EGLint iConfigs;
    ret = eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1);
    GCLAssertMsg(ret, "eglChooseConfig() failed.");

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
    if (eglSurface == EGL_NO_SURFACE)
    {    
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)NULL, NULL);
	}



#if defined(ES2)
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
#else
#error "TBD"
#endif
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
	eglErrorCheck();

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	eglErrorCheck();

	int i32Values;
	eglGetConfigAttrib(eglDisplay, eglConfig, EGL_DEPTH_SIZE , &i32Values);

	Init3DState();
	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
	const char *ver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);glErrorCheck();
	mShadingLanguageVersion = std::string(ver);
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

}
GLESRenderer::~GLESRenderer()
{
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ;
	eglTerminate(eglDisplay);

#ifdef OS_WIN32
	ReleaseDC( mhWnd, mhDC );
#elif defined(OS_LINUX)
	if (mWin)
		XDestroyWindow(mDisplay, mWin);
    if (mCmap)
    	XFreeColormap( mDisplay, mCmap);

    delete x11Visual;

#endif
}

bool GLESRenderer::Update()
{
	return true;
}


void GLESRenderer::PreRender()
{
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
}
void GLESRenderer::PostRender()
{
}


void GLESRenderer::RenderState::SetTextureEnabled(bool isEnabled)
{
	if (isEnabled)
	{
		glEnable(GL_TEXTURE_2D); glErrorCheck();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
		glDisable(GL_TEXTURE_2D); glErrorCheck();
	}
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
	eglSwapBuffers(eglDisplay, eglSurface);
}

void GCL::GLESRenderer::SetProjection( const Camera *camera )
{
	mProjection.SetPerspective(camera->GetFov(),camera->GetAspectRatio(),camera->GetNear(),camera->GetFar());
	mModelView= Inverse(camera->GetTransform());
}
