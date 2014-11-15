/*
* Copyright (C) 2014 by Francois Coulombe
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
#ifdef OS_ANDROID
#include "windriver/WinDriver.h"

#include <sstream>
#include <map>

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/Log.h>
#include <gcl/Time.h>
#include "windriver/EventHandler.h"
#include <native_app_glue/android_native_app_glue.h>

namespace GCL
{

extern android_app* gAndroidApp;
	class pWinDriver
	{
	public:
		pWinDriver(const char *windowsTitle)
			: mPreviousFrameTime(0.0),
			mFPS(0),
			mAccumulatedTime(0.0),
			mDt(0.0),
			mIsReady(false)
		{


			mWindowsTitle = windowsTitle;
			gAndroidApp->userData = this;
			gAndroidApp->onAppCmd = HandleEvent;
			gAndroidApp->onInputEvent = HandleInputs;

		}

		~pWinDriver()
		{

		}
		static void HandleEvent(struct android_app* app, int32_t cmd)
		{
		    pWinDriver* window = (pWinDriver*)app->userData;
		    switch (cmd) {
		        case APP_CMD_SAVE_STATE:
		            // The system has asked us to save our current state.  Do so.
		        	//gAndroidApp->savedState = malloc(sizeof(struct saved_state));
		            //*((struct saved_state*)gAndroidApp->savedState) = engine->state;
		            //gAndroidApp->savedStateSize = sizeof(struct saved_state);
		            break;
		        case APP_CMD_INIT_WINDOW:
		            // The window is being shown, get it ready.
		            if (gAndroidApp->window != NULL)
		            {
		            	window->mIsReady = true;
		            }
		            break;
		        case APP_CMD_TERM_WINDOW:
		            // The window is being hidden or closed, clean it up.
		            KLog("engine_term_display(engine);");
		            break;
		        case APP_CMD_GAINED_FOCUS:
		            // When our app gains focus, we start monitoring the accelerometer.
		            /*if (engine->accelerometerSensor != NULL) {
		                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
		                        engine->accelerometerSensor);
		                // We'd like to get 60 events per second (in us).
		                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
		                        engine->accelerometerSensor, (1000L/60)*1000);
		            }*/
		            break;
		        case APP_CMD_LOST_FOCUS:
		            // When our app loses focus, we stop monitoring the accelerometer.
		            // This is to avoid consuming battery while not being used.
		            /*if (engine->accelerometerSensor != NULL) {
		                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
		                        engine->accelerometerSensor);
		            }
		            // Also stop animating.
		            engine->animating = 0;*/
		            KLog("engine_draw_frame(engine);");
		            break;
		    }
		}
		static int32_t HandleInputs(struct android_app* /*app*/, AInputEvent* /*event*/)
		{
		    //pWinDriver* engine = (pWinDriver*)app->userData;
		    /*if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		        engine->animating = 1;
		        engine->state.x = AMotionEvent_getX(event, 0);
		        engine->state.y = AMotionEvent_getY(event, 0);
		        return 1;
		    }*/
		    return 0;
		}
		size_t GetWindowsHandle() const
		{
			return (size_t)gAndroidApp->window;
		}
		void SwapBuffer()
		{   
		       // Read all pending events.
		        int ident;
		        int events;
		        struct android_poll_source* source;

		        // If not animating, we will block forever waiting for events.
		        // If animating, we loop until all events are read, then continue
		        // to draw the next frame of animation.
		        while ((ident=ALooper_pollAll(0, NULL, &events,
		                (void**)&source)) >= 0) {

		            // Process this event.
		            if (source != NULL) {
		                source->process(gAndroidApp, source);
		            }

		            // If a sensor has data, process it now.
		            /*if (ident == LOOPER_ID_USER) {
		                if (engine.accelerometerSensor != NULL) {
		                    ASensorEvent event;
		                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
		                            &event, 1) > 0) {
		                        LOGI("accelerometer: x=%f y=%f z=%f",
		                                event.acceleration.x, event.acceleration.y,
		                                event.acceleration.z);
		                    }
		                }
		            }*/

		            // Check if we are exiting.
		            /*if (gAndroidApp->destroyRequested != 0) {
		                engine_term_display(&engine);
		                return;
		            }*/
		        }

		        /*if (engine.animating) {
		            // Done with events; draw next animation frame.
		            engine.state.angle += .01f;
		            if (engine.state.angle > 1) {
		                engine.state.angle = 0;
		            }

		            // Drawing is throttled to the screen update rate, so there
		            // is no need to do timing here.
		            engine_draw_frame(&engine);
		        }*/

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

		bool IsReady() const { return mIsReady; }
	private:
		std::string mWindowsTitle;
		double mPreviousFrameTime;
		size_t mFPS;
		double mAccumulatedTime;
		Real mDt;
		bool mIsReady;
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
		return mpWinDriver->IsReady();
	}
}
#endif
