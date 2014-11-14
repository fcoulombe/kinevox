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
#include <gcl/Time.h>
#include "windriver/EventHandler.h"

namespace GCL
{


	class pWinDriver
	{
	public:
		pWinDriver(const char *windowsTitle)
			: mPreviousFrameTime(0.0),
			mFPS(0),
			mAccumulatedTime(0.0),
			mDt(0.0)
		{


			mWindowsTitle = windowsTitle;
		}

		~pWinDriver()
		{

		}

		size_t GetWindowsHandle() const
		{
			return 0;
		}
		void SwapBuffer()
		{   

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
