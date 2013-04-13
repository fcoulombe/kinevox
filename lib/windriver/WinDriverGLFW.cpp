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
#ifdef USE_GLFW
#include "windriver/WinDriver.h"

#include <sstream>
#include <map>
#include <3rdparty/GLFW.h>
#include <3rdparty/OpenGL.h>

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include "windriver/EventHandler.h"

namespace GCL
{

void GLFWCALL KeyCallBack(int key,int state)
{
    if (state == GLFW_PRESS)
    {
        EventManager::Instance().KeyDown(key);
    }
    else if (state == GLFW_RELEASE)
    {
        EventManager::Instance().KeyUp(key);
    }
    else
    {
        GCLAssert(false);
    }
}

void GLFWCALL MouseMoveCallback(int /*i*/ ,int /*j*/)
{

}
void GLFWCALL MouseButtonCallback(int /*i*/ ,int /*j*/)
{

}
void GLFWCALL MouseWheelCallback(int /*i*/ )
{

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
        int ret = glfwInit(); 
        GCLAssert(ret);

        int width = Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
        int height = Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");

        ret = glfwOpenWindow( width,height, 0,0,0,0,24,8, GLFW_WINDOW );
        GCLAssert(ret);

        mWindowsTitle = windowsTitle;
        glfwSetWindowTitle( windowsTitle ); 
        glfwSetKeyCallback( KeyCallBack );
        glfwSetMousePosCallback( MouseMoveCallback);
        glfwSetMouseButtonCallback( MouseButtonCallback);
        glfwSetMouseWheelCallback( MouseWheelCallback );
    }

    ~pWinDriver()
    {
        glfwTerminate();
    }

    void SwapBuffer()
    {
        glfwSwapBuffers();glErrorCheck();
        //calculate fps
        double currentTime = glfwGetTime();
        double dt = currentTime - mPreviousFrameTime;
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
            glfwSetWindowTitle( fps.str().c_str());
            mFPS=0;
        }
        mPreviousFrameTime = currentTime;
        
    }

    Real GetDt() const { return mDt; }
    size_t GetWindowsHandle() const
    {
        GCLAssert(false && "use WinAPI");
    }
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
    return 0;
}
}
#endif