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


#include "renderer/GL/GLRenderer.h"

#include <sstream>

#include <3rdparty/OpenGL.h>
#include <gcl/Assert.h>
#include <gcl/StringUtil.h>

#include "renderer/Camera.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/Text2D.h"
#include "renderer/GL/GLGPUProgram.h"
#include "renderer/GL/GLRenderUtils.h"
#include "renderer/GL/GLShader.h"
#include "renderer/GL/GLVertexBuffer.h"

#ifdef OS_WIN32
#include <GL/wglext.h>
#elif defined(OS_LINUX)
#include <GL/glx.h>
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
#endif
using namespace GCL;

void GLRenderer::Init3DState()
{
	glErrorCheck();
    glViewport(0,0,(GLsizei)mViewPort.GetWidth(),(GLsizei)mViewPort.GetHeight()); 
	glErrorCheck();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
	glClearDepth(1.0); glErrorCheck();
	glDepthMask(GL_TRUE); glErrorCheck();
	glDepthFunc(GL_LESS); glErrorCheck();
	glEnable(GL_DEPTH_TEST); glErrorCheck();
	glEnable(GL_BLEND); glErrorCheck();

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
	

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); glErrorCheck();
}
#ifdef OS_LINUX
// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)

{
	std::vector<std::string> extVector;
	StringUtil::Explode(extList, extVector, ' ');
	for (size_t i=0; i<extVector.size(); ++i)
	{
		if (extVector[i] == extension)
			return true;
	}
	return false;
}

static int ctxErrorHandler( Display *, XErrorEvent * )
{
	GCLAssert(false);
	return 0;
}
#endif
void GLRenderer::InitGLX(size_t windowsHandle)
{
#ifdef OS_LINUX
	mDisplay = (Display *)*(size_t*)windowsHandle;
	mWin = *(Window*)(((uint8_t*)windowsHandle)+sizeof(Display *));

	// Get a matching FB config
	static const int visual_attribs[] =
	{
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			GLX_ALPHA_SIZE      , 8,
			GLX_DEPTH_SIZE      , 24,
			GLX_STENCIL_SIZE    , 8,
			GLX_DOUBLEBUFFER    , True,
			//GLX_SAMPLE_BUFFERS  , 1,
			//GLX_SAMPLES         , 4,
			None
	};

	int glx_major, glx_minor;

	// FBConfigs were added in GLX version 1.3.
	int ret = glXQueryVersion( mDisplay, &glx_major, &glx_minor );
	GCLAssertMsg(ret && ( glx_major >= 1 ) && ( glx_minor >= 3 ) , "Invalid GLX version" );

	int fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig( mDisplay, DefaultScreen( mDisplay ),visual_attribs, &fbcount );
	GCLAssertMsg( fbc ,"Failed to retrieve a framebuffer config\n" );

	//std::cout <<  "Found "<<fbcount <<" matching FB configs."<<std::endl;

	// Pick the FB config/visual with the most samples per pixel
	int best_fbc = -1,
			worst_fbc = -1,
			best_num_samp = -1,
			worst_num_samp = 999;


	for ( int i = 0; i < fbcount; i++ )
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig( mDisplay, fbc[i] );
		if ( vi )
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib( mDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( mDisplay, fbc[i], GLX_SAMPLES       , &samples  );
#if 0
			std::cout << "  Matching fbconfig " << i
					<< ", visual ID "<<vi->visualid
					<<": SAMPLE_BUFFERS = "<< samp_buf
					<<", SAMPLES = " << samples << std::endl;
#endif
			if ( (best_fbc < 0) || ((samp_buf) && (samples > best_num_samp)) )
			{
				best_fbc = i;
				best_num_samp = samples;
			}
			if ( (worst_fbc < 0) || (!samp_buf) || (samples < worst_num_samp) )
			{
				worst_fbc = i;
				worst_num_samp = samples;
			}
		}
		XFree( vi );
	}


	GLXFBConfig bestFbc = fbc[ best_fbc ];

	// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree( fbc );


	// Get the default screen's GLX extension list
	const char *glxExts = glXQueryExtensionsString( mDisplay,DefaultScreen( mDisplay ) );

	// NOTE: It is not necessary to create or make current to a context before
	// calling glXGetProcAddressARB
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
		            				 glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

	mCtx = 0;

	// Install an X error handler so the application won't exit if GL 3.0
	// context allocation fails.
	//
	// Note this error handler is global.  All display connections in all threads
	// of a process use the same error handler, so be sure to guard against other
	// threads issuing X commands while this code is running.
	bool ctxErrorOccurred = false;
	int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

	// Check for the GLX_ARB_create_context extension string and the function.
	// If either is not present, use GLX 1.3 context creation method.
	if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||
			!glXCreateContextAttribsARB )
	{
		printf( "glXCreateContextAttribsARB() not found"
				" ... using old-style GLX context\n" );
		mCtx = glXCreateNewContext( mDisplay, bestFbc, GLX_RGBA_TYPE, 0, True );
	}

	// If it does, try to get a GL 3.0 context!
	else
	{
#if USE_OPENGL3
		int context_attribs[] =
		{
				GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, 1,
				GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
				None
		};


		mCtx = glXCreateContextAttribsARB( mDisplay, bestFbc, 0,True, context_attribs );

		// Sync to ensure any errors generated are processed.
		XSync( mDisplay, False );
		if ( !ctxErrorOccurred && mCtx )
		{
			//std::cout << "Created GL 3.0 context" << std::endl;
		}
		else
		{
			// Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
			// When a context version below 3.0 is requested, implementations will
			// return the newest context version compatible with OpenGL versions less
			// than version 3.0.
			// GLX_CONTEXT_MAJOR_VERSION_ARB = 1
			context_attribs[1] = 1;
			// GLX_CONTEXT_MINOR_VERSION_ARB = 0
			context_attribs[3] = 0;

			ctxErrorOccurred = false;

			KLog("Failed to create GL 3.0 context  ... using old-style GLX context");
			mCtx = glXCreateContextAttribsARB( mDisplay, bestFbc, 0,True, context_attribs );
		}
#else
		mCtx = glXCreateNewContext( mDisplay, bestFbc, GLX_RGBA_TYPE, 0, True );
#endif
	}

	// Sync to ensure any errors generated are processed.
	XSync( mDisplay, False );

	// Restore the original error handler
	XSetErrorHandler( oldHandler );

	GCLAssertMsg ( !ctxErrorOccurred && mCtx, "Failed to create an OpenGL context\n" );


	// Verifying that context is a direct context
	if ( ! glXIsDirect ( mDisplay, mCtx ) )
	{
		KLog("Indirect GLX rendering context obtained");
	}
	else
	{
		//std::cout << "Direct GLX rendering context obtained" << std::endl;
	}

	glXMakeCurrent( mDisplay, mWin, mCtx );
#else
	(void)windowsHandle;
#endif
}
void GLRenderer::InitWGL(size_t windowsHandle)
{
#ifdef OS_WIN32
	// remember the window handle (HWND)
	mhWnd = (HWND)windowsHandle;

	// get the device context (DC)
	mhDC = GetDC( mhWnd );
	GCLAssertMsg(mhDC, "Failed to get the device context");

	// set the pixel format for the DC
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat( mhDC, &pfd );
	BOOL wglRet = SetPixelFormat( mhDC, format, &pfd );
	HGLRC tempContext = wglCreateContext(mhDC); 
	wglRet = wglMakeCurrent(mhDC,tempContext);

#if USE_OPENGL3
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	if(wglCreateContextAttribsARB != NULL)
	{
		mhRC = wglCreateContextAttribsARB(mhDC,0, attribs);
		wglMakeCurrent(NULL,NULL); 
		wglDeleteContext(tempContext);
		wglRet = wglMakeCurrent( mhDC, mhRC );
	}
	else
#endif
	{
		// fall back on normal context
		mhRC = tempContext;
	}
	// make it the current render context
#else
	(void)windowsHandle;
#endif
}


GLRenderer::GLRenderer(size_t windowsHandle)
	:   mModelView(true),
			mFov(45.0),
	mAspect(640.0/480.0),
	mNear(0.1),
	mFar(100.0)
{

	RenderPipe::SendCommand([this, windowsHandle](){
	mProjection.SetPerspective(mFov,mAspect,mNear,mFar);

	InitWGL(windowsHandle);
	InitGLX(windowsHandle);

#if ENABLE_GLEW
	glewExperimental=1;
	GLenum err = glewInit();
	GCLAssertMsg(GLEW_OK == err, (const char *)glewGetErrorString(err));
	mGlewVersion = std::string((const char*)glewGetString(GLEW_VERSION));
	glGetError(); //glew generates an error that can be ignored. on 3.x+
	glErrorCheck();
#else
	mGlewVersion  = std::string("Unused");
#endif


	Init3DState();
	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
	const char *ver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);glErrorCheck();
	mShadingLanguageVersion = std::string(ver);
	char delim = ' ';

	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);glErrorCheck();
	std::string extString;
	for (i = 0; i < n; i++)
	{
		const char *ex = (const char*)glGetStringi(GL_EXTENSIONS, i); glErrorCheck();
		extString = extString +ex;
		extString = extString + " ";
	}
	
	StringUtil::Explode(extString, mExtensions ,delim); 
	});

}
GLRenderer::~GLRenderer()
{

	RenderPipe::SendCommandSync([&](){
	mExtensions.clear();
#ifdef OS_WIN32
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( mhRC );
	ReleaseDC( mhWnd, mhDC );
#elif defined(OS_LINUX)
	glXMakeCurrent( mDisplay, 0, 0 );
	glXDestroyContext( mDisplay, mCtx );
#endif
	});
}

bool GLRenderer::Update()
{
	return true;
}


void GLRenderer::PreRender()
{
	RenderPipe::SendCommand([&](){
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
	});
}
void GLRenderer::PostRender()
{
}


void GLRenderer::RenderState::SetTextureEnabled(bool isEnabled)
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


Matrix44 GLRenderer::GetGLProjection()
{
    Matrix44 projectionMatrixd;
	RenderPipe::SendCommandSync([&](){
        Matrix44f projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);

	projectionMatrixd[0].x = projectionMatrix.m0.x;
	projectionMatrixd[0].y = projectionMatrix.m0.y;
	projectionMatrixd[0].z = projectionMatrix.m0.z;
	projectionMatrixd[0].w = projectionMatrix.m0.w;
	projectionMatrixd[1].x = projectionMatrix.m1.x;
	projectionMatrixd[1].y = projectionMatrix.m1.y;
	projectionMatrixd[1].z = projectionMatrix.m1.z;
	projectionMatrixd[1].w = projectionMatrix.m1.w;
	projectionMatrixd[2].x = projectionMatrix.m2.x;
	projectionMatrixd[2].y = projectionMatrix.m2.y;
	projectionMatrixd[2].z = projectionMatrix.m2.z;
	projectionMatrixd[2].w = projectionMatrix.m2.w;
	projectionMatrixd[3].x = projectionMatrix.m3.x;
	projectionMatrixd[3].y = projectionMatrix.m3.y;
	projectionMatrixd[3].z = projectionMatrix.m3.z;
	projectionMatrixd[3].w = projectionMatrix.m3.w;
	});

	return projectionMatrixd;
}
Matrix44 GLRenderer::GetGLModelView()
{
    Matrix44 modelViewMatrixd;

	RenderPipe::SendCommandSync([&](){
	Matrix44f modelViewMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&modelViewMatrix);
	
	modelViewMatrixd[0].x = modelViewMatrix.m0.x;
	modelViewMatrixd[0].y = modelViewMatrix.m0.y;
	modelViewMatrixd[0].z = modelViewMatrix.m0.z;
	modelViewMatrixd[0].w = modelViewMatrix.m0.w;
	modelViewMatrixd[1].x = modelViewMatrix.m1.x;
	modelViewMatrixd[1].y = modelViewMatrix.m1.y;
	modelViewMatrixd[1].z = modelViewMatrix.m1.z;
	modelViewMatrixd[1].w = modelViewMatrix.m1.w;
	modelViewMatrixd[2].x = modelViewMatrix.m2.x;
	modelViewMatrixd[2].y = modelViewMatrix.m2.y;
	modelViewMatrixd[2].z = modelViewMatrix.m2.z;
	modelViewMatrixd[2].w = modelViewMatrix.m2.w;
	modelViewMatrixd[3].x = modelViewMatrix.m3.x;
	modelViewMatrixd[3].y = modelViewMatrix.m3.y;
	modelViewMatrixd[3].z = modelViewMatrix.m3.z;
	modelViewMatrixd[3].w = modelViewMatrix.m3.w;
	});
	return modelViewMatrixd;
}

void GLRenderer::SwapBuffer()
{

	RenderPipe::SendCommand([&](){
#ifdef OS_WIN32
	SwapBuffers( mhDC );
#elif defined(OS_LINUX)
	glXSwapBuffers ( mDisplay, mWin );
#endif
	});
}

void GCL::GLRenderer::SetProjection( const Camera &camera )
{
	RenderPipe::SendCommand([&](){
	mProjection.SetPerspective(camera.GetFov(),camera.GetAspectRatio(),camera.GetNear(),camera.GetFar());
	mModelView= Inverse(camera.GetTransform());
	});
}

void GCL::GLRenderer::SetIsDepthTesting( bool isDepthTesting /*= true*/ )
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
