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
#ifdef OS_LINUX
#include "windriver/WinDriver.h"

#include <sstream>
#include <map>
#include <vector>
#include <GL/gl.h>
#include <GL/glx.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/StringUtil.h>
#include <gcl/Time.h>
#include "windriver/EventHandler.h"

namespace GCL
{
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

class pWinDriver
{
public:
	pWinDriver(const char *windowsTitle)
: mPreviousFrameTime(0.0),
  mFPS(0),
  mAccumulatedTime(0.0),
  mDt(0.0)
{
		mDisplay = XOpenDisplay(0);
		GCLAssert(mDisplay);

		// Get a matching FB config
		static int visual_attribs[] =
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
		if ( !glXQueryVersion( mDisplay, &glx_major, &glx_minor ) ||
				( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
		{
			printf( "Invalid GLX version" );
			exit(1);
		}

		printf( "Getting matching framebuffer configs\n" );
		int fbcount;
		GLXFBConfig *fbc = glXChooseFBConfig( mDisplay, DefaultScreen( mDisplay ),
				visual_attribs, &fbcount );
		GCLAssertMsg( fbc ,"Failed to retrieve a framebuffer config\n" );

		std::cout <<  "Found "<<fbcount <<" matching FB configs."<<std::endl;

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

				std::cout << "  Matching fbconfig " << i
						<< ", visual ID "<<vi->visualid
						<<": SAMPLE_BUFFERS = "<< samp_buf
						<<", SAMPLES = " << samples << std::endl;

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

		// Get a visual
		XVisualInfo *vi = glXGetVisualFromFBConfig( mDisplay, bestFbc );
		std::cout <<  "Chosen visual ID = " << vi->visualid <<std::endl;



		XSetWindowAttributes swa;

		swa.colormap = mCmap = XCreateColormap( mDisplay,
				RootWindow( mDisplay, vi->screen ),
				vi->visual, AllocNone );
		swa.background_pixmap = None ;
		swa.border_pixel      = 0;
		swa.event_mask        = StructureNotifyMask;



		mWin = XCreateWindow( mDisplay, RootWindow( mDisplay, vi->screen ),
				0, 0, 100, 100, 0, vi->depth, InputOutput,
				vi->visual,
				CWBorderPixel|CWColormap|CWEventMask, &swa );
		GCLAssertMsg( !mWin , "Failed to create window." );


		// Done with the visual info data
		XFree( vi );

		XStoreName( mDisplay, mWin, "GL 3.0 Window" );


		XMapWindow( mDisplay, mWin );

		// Get the default screen's GLX extension list
		const char *glxExts = glXQueryExtensionsString( mDisplay,
				DefaultScreen( mDisplay ) );

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
			int context_attribs[] =
			{
					GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
					GLX_CONTEXT_MINOR_VERSION_ARB, 0,
					//GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					None
			};


			mCtx = glXCreateContextAttribsARB( mDisplay, bestFbc, 0,
					True, context_attribs );

			// Sync to ensure any errors generated are processed.
			XSync( mDisplay, False );
			if ( !ctxErrorOccurred && mCtx )
				std::cout << "Created GL 3.0 context" << std::endl;
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

				std::cout <<  "Failed to create GL 3.0 context"
						" ... using old-style GLX context" << std::endl;
				mCtx = glXCreateContextAttribsARB( mDisplay, bestFbc, 0,
						True, context_attribs );
			}
		}

		// Sync to ensure any errors generated are processed.
		XSync( mDisplay, False );

		// Restore the original error handler
		XSetErrorHandler( oldHandler );

		GCLAssertMsg ( !ctxErrorOccurred && mCtx, "Failed to create an OpenGL context\n" );


		// Verifying that context is a direct context
		if ( ! glXIsDirect ( mDisplay, mCtx ) )
		{
			std::cout << "Indirect GLX rendering context obtained" << std::endl;
		}
		else
		{
			std::cout << "Direct GLX rendering context obtained" << std::endl;
		}

		glXMakeCurrent( mDisplay, mWin, mCtx );

		mWindowsTitle = windowsTitle;
}

	~pWinDriver()
	{

		glXMakeCurrent( mDisplay, 0, 0 );
		glXDestroyContext( mDisplay, mCtx );

		XDestroyWindow( mDisplay, mWin );
		XFreeColormap( mDisplay, mCmap );
		XCloseDisplay( mDisplay );

	}

	size_t GetWindowsHandle() const
	{
		return (size_t)mDisplay;
	}

	void ProcessEvents()
	{
		XEvent event;
		/* handle the events in the queue */
		while (XPending(mDisplay) > 0)
		{
			XNextEvent(mDisplay, &event);
			switch (event.type)
			{
			case Expose:
				if (event.xexpose.count != 0)
					break;
				//drawGLScene();
				break;
			case ConfigureNotify:
				/* call resizeGLScene only if our window-size changed */

				break;
				/* exit in case of a mouse button press */
			case ButtonPress:

				break;
			case KeyPress:
				EventManager::Instance().KeyDown((uint32_t)XLookupKeysym(&event.xkey, 0));
				break;
			case ClientMessage:
				if (*XGetAtomName(mDisplay, event.xclient.message_type) ==
						*"WM_PROTOCOLS")
				{
					printf("Exiting sanely...\n");

				}
				break;
			default:
				break;
			}
		}


	}
	void SwapBuffer()
	{
		glXSwapBuffers ( mDisplay, mWin );

		ProcessEvents();
		//calculate fps
		double currentTime = Time::GetTickMs()/1000.0;
		double dt = currentTime - mPreviousFrameTime;
		//std::cout << dt << std::endl;
		mAccumulatedTime += dt;
		mFPS++;
		mDt = dt;
		if (mAccumulatedTime>1.0)
		{
			mAccumulatedTime = 0.0;
			std::stringstream fps;
			fps << mWindowsTitle;
			fps<< " - FPS: ";
			fps<< mFPS;
			//SetWindowText(mWindowsHandle, fps.str().c_str());
			mFPS=0;
		}
		mPreviousFrameTime = currentTime;

	}

	Real GetDt() const { return mDt; }

private:
	Display *mDisplay;
	Window mWin;
	GLXContext mCtx;
	Colormap mCmap;
	//HDC	 hDC;
	std::string mWindowsTitle;
	double mPreviousFrameTime;
	size_t mFPS;
	double mAccumulatedTime;
	Real mDt;
};
WinDriver::WinDriver(const char *windowsTitle)
{
	EventManager::Initialize();
	mpWinDriver = new pWinDriver(windowsTitle);
}
WinDriver::~WinDriver()
{
	delete mpWinDriver;
	EventManager::Terminate();
}
void WinDriver::SwapBuffer()
{
	mpWinDriver->SwapBuffer();
}
Real WinDriver::GetDt() const
{
	return mpWinDriver->GetDt();
}

size_t WinDriver::GetWindowsHandle() const
{
	return (size_t)mpWinDriver->GetWindowsHandle();
}
}
#endif
