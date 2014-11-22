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
#include <GL/glx.h>
#include <X11/Xlib.h>

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/Log.h>
#include <gcl/StringUtil.h>
#include <gcl/Time.h>
#include "windriver/EventHandler.h"

namespace GCL
{
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
Bool checkEvent(::Display*, XEvent* event, XPointer userData)
{
	// Just check if the event matches the window
	return event->xany.window == reinterpret_cast< ::Window >(userData);
}
class pWinDriver
{
public:
	pWinDriver(const char *windowsTitle)
: mPreviousFrameTime(0.0),
  mFPS(0),
  mAccumulatedTime(0.0),
  mDt(0.0),
  mIsInFocus(true)
{
		Status stats = XInitThreads();
		GCLAssert(stats != 0);
		data.mDisplay = XOpenDisplay(0);
		GCLAssert(data.mDisplay);

		mScreen = DefaultScreen(data.mDisplay);

		mWindowsTitle = windowsTitle;

		int glx_major, glx_minor;

		// FBConfigs were added in GLX version 1.3.
		int ret = glXQueryVersion( data.mDisplay, &glx_major, &glx_minor );
		GCLAssertMsg(ret && ( glx_major >= 1 ) && ( glx_minor >= 3 ) , "Invalid GLX version" );
		int fbcount;
		GLXFBConfig *fbc = glXChooseFBConfig( data.mDisplay, DefaultScreen( data.mDisplay ),visual_attribs, &fbcount );
		GCLAssertMsg( fbc ,"Failed to retrieve a framebuffer config\n" );

		//std::cout <<  "Found "<<fbcount <<" matching FB configs."<<std::endl;

		// Pick the FB config/visual with the most samples per pixel
		int best_fbc = -1,
				worst_fbc = -1,
				best_num_samp = -1,
				worst_num_samp = 999;


		for ( int i = 0; i < fbcount; i++ )
		{
			XVisualInfo *vi = glXGetVisualFromFBConfig( data.mDisplay, fbc[i] );
			if ( vi )
			{
				int samp_buf, samples;
				glXGetFBConfigAttrib( data.mDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
				glXGetFBConfigAttrib( data.mDisplay, fbc[i], GLX_SAMPLES       , &samples  );
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

		// Get a visual
		XVisualInfo *vi = glXGetVisualFromFBConfig( data.mDisplay, bestFbc );
		//std::cout <<  "Chosen visual ID = " << vi->visualid <<std::endl;


		XSetWindowAttributes swa;

		swa.colormap = mCmap = XCreateColormap( data.mDisplay,RootWindow( data.mDisplay, vi->screen ),vi->visual, AllocNone );
		swa.background_pixmap = None ;
		swa.border_pixel      = 0;
		swa.event_mask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                EnterWindowMask | LeaveWindowMask;;

		int width = Config::Instance().GetInt("DEFAULT_SCREEN_WIDTH");
		int height = Config::Instance().GetInt("DEFAULT_SCREEN_HEIGHT");
		data.mWin = XCreateWindow( data.mDisplay, RootWindow( data.mDisplay, vi->screen ),
				0, 0, width, height, 0, vi->depth, InputOutput,
				vi->visual,
				CWBorderPixel|CWColormap|CWEventMask, &swa );
		GCLAssertMsg( data.mWin , "Failed to create window." );

		// Done with the visual info data
		XFree( vi );

		XStoreName( data.mDisplay, data.mWin, mWindowsTitle.c_str());

		// Get the atom defining the close event
		mAtomClose = XInternAtom(data.mDisplay, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(data.mDisplay, data.mWin, &mAtomClose, 1);

		// Create the input context
		mInputMethod = XOpenIM(data.mDisplay, NULL, NULL, NULL);
		GCLAssert(mInputMethod);
		mInputContext = XCreateIC(mInputMethod,
				XNClientWindow, data.mWin,
				XNFocusWindow,  data.mWin,
				XNInputStyle,   XIMPreeditNothing  | XIMStatusNothing,
				(void*)NULL);
		GCLAssert(mInputContext);

		XMapWindow( data.mDisplay, data.mWin );
		XFlush(data.mDisplay);
}

	~pWinDriver()
	{
		XDestroyIC(mInputContext);

		XDestroyWindow( data.mDisplay, data.mWin );
		XFlush(data.mDisplay);
		XCloseIM(mInputMethod);
		XFreeColormap( data.mDisplay, mCmap );
		XCloseDisplay( data.mDisplay );
	}

	size_t GetWindowsHandle() const
	{
		return (size_t)&data;
	}

	void ProcessEvents()
	{
		XEvent event;
		while (XCheckIfEvent(data.mDisplay, &event, &checkEvent, reinterpret_cast<XPointer>(data.mWin)))
		{

			switch (event.type)
			{
			case Expose:
				//std::cout << "expose!" << std::endl;
				if (event.xexpose.count != 0)
					break;
				//drawGLScene();
				break;
			case ConfigureNotify:
				/* call resizeGLScene only if our window-size changed */
				//std::cout << "config!" << std::endl;
				break;
				// Destroy event
			case DestroyNotify :
			{
				// The window is about to be destroyed : we must cleanup resources
				//cleanup();
				break;
			}
			case FocusIn :
			{
				SetInFocus();
				// Update the input context
				if (mInputContext)
					XSetICFocus(mInputContext);

				//Event event;
				//event.type = Event::GainedFocus;
				//pushEvent(event);
				break;
			}
			case FocusOut :
			{
				SetInFocus(false);
				KLog("lose focus");
				// Update the input context
				if (mInputContext)
					XUnsetICFocus(mInputContext);

				//Event event;
				//event.type = Event::LostFocus;
				//pushEvent(event);
				break;
			}
			/* exit in case of a mouse button press */
			case ButtonPress:
				EventManager::Instance().MouseDown(event.xbutton.x, event.xbutton.y);
				break;
			case ButtonRelease:
				EventManager::Instance().MouseUp(event.xbutton.x, event.xbutton.y);
				break;
			case KeyRelease:
				EventManager::Instance().KeyUp((uint32_t)XLookupKeysym(&event.xkey, 0));
				break;
			case KeyPress:
				EventManager::Instance().KeyDown((uint32_t)XLookupKeysym(&event.xkey, 0));
				break;
				// Mouse moved
			case MotionNotify :
			{
				//KLog("we're in motion!");
				//EventManager::Instance().ClickDown()
				//Event event;
				//event.type        = Event::MouseMoved;
				//event.mouseMove.x = windowEvent.xmotion.x;
				//event.mouseMove.y = windowEvent.xmotion.y;
				//pushEvent(event);
				break;
			}
			// Mouse entered
			case EnterNotify :
			{
				if (event.xcrossing.mode == NotifyNormal)
				{
					//Event event;
					//event.type = Event::MouseEntered;
					//pushEvent(event);
				}
				break;
			}
			// Mouse left
			case LeaveNotify :
			{
				if (event.xcrossing.mode == NotifyNormal)
				{
					//Event event;
					//event.type = Event::MouseLeft;
					//pushEvent(event);
				}
				break;
			}
			case ClientMessage:
				//std::cout << "clientmessage!" << std::endl;
				if (*XGetAtomName(data.mDisplay, event.xclient.message_type) ==
						*"WM_PROTOCOLS")
				{
					printf("Exiting sanely...\n");
					EventManager::Instance().KeyUp((uint32_t)XK_Escape);

				}
				break;
			default:
				//std::cout << "unhandled!" << std::endl;
				break;
			}
		}
	}
	void SwapBuffer()
	{
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
			XStoreName( data.mDisplay, data.mWin, fps.str().c_str());
			mFPS=0;
		}
		mPreviousFrameTime = currentTime;

	}

	Real GetDt() const { return mDt; }
	bool IsInFocus() const { return mIsInFocus; }
	void SetInFocus(bool isInFocus = true) { mIsInFocus = isInFocus; }
private:
	struct WindowsData
	{
		Display *mDisplay;
		Window mWin;

	};
	WindowsData data;
	Atom       mAtomClose;
	Colormap mCmap;
	XIM        mInputMethod;  ///< Input method linked to the X display
	XIC        mInputContext; ///< Input context used to get unicode input in our window
	int     mScreen;
	//HDC	 hDC;
	std::string mWindowsTitle;
	double mPreviousFrameTime;
	size_t mFPS;
	double mAccumulatedTime;
	Real mDt;
	bool mIsInFocus;
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
bool WinDriver::IsReady() const
{
	return true;
}
bool WinDriver::IsInFocus() const
{
	return mpWinDriver->IsInFocus();
}
}
#endif
