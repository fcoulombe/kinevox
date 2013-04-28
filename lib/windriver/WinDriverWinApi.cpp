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
#ifdef USE_WINAPI
#include "windriver/WinDriver.h"

#include <sstream>
#include <map>
#include <windows.h>
#pragma comment(lib, "User32.lib") 

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include "windriver/EventHandler.h"

namespace GCL
{
#if 0
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
#endif
    LRESULT CALLBACK wndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) //callback function for our GetMessage function on the while statement inside the main function
    {
        switch(uMsg) //switch the MSG variable passed
        {
           /*
			Here we are handling 2 system messages: screen saving and monitor power.
			They are especially relevent on mobile devices.
		*/
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:					// Screensaver trying to start ?
				case SC_MONITORPOWER:				// Monitor trying to enter powersave ?
				return 0;							// Prevent this from happening
			}
			break;
		}
        case WM_CLOSE: //case it's a Window Close MSG
            PostQuitMessage(0); //Apply a Quit Message
            break;
        }
        return DefWindowProc(hWnd,uMsg,wParam,lParam); //pass the variable
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
            mInstance = GetModuleHandle(NULL);
            WNDCLASSEX wc; //window class won't go in detail not related to this tutorial really
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc = (WNDPROC)wndProc;
            wc.cbWndExtra = 0;
            wc.cbClsExtra = 0;
            wc.hInstance = mInstance;
            wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = "D3DTEST"; //class name
            wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);



            int width = Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
            int height = Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");

            RECT rect; //window rectangle
            rect.top = (long)0;
            rect.left = (long)0;
            rect.right = width; 
            rect.bottom = height; 
            BOOL ret = RegisterClassEx(&wc);
            GCLAssertMsg(ret, "Could not register window class");

            AdjustWindowRectEx(&rect,WS_OVERLAPPEDWINDOW,false,WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
            mWindowsHandle = CreateWindowEx(NULL,"D3DTEST","D3D TEST",WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
                0,0,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,mInstance,NULL);
            if(!mWindowsHandle) //if the window failed to create
            {
                DestroyWindow(mWindowsHandle); //destroy the window
                UnregisterClass("D3DTEST",mInstance); //unregister our window class
                GCLAssertMsg(false, "Failed to create the window"); //error pop-up for debug purpose
            }

            // Get the associated device context
            hDC = GetDC(hWnd);
            if (!hDC)
            {
                DestroyWindow(mWindowsHandle); //destroy the window
                UnregisterClass("D3DTEST",mInstance); //unregister our window class
                GCLAssertMsg(false, "Failed to create the device context");
            }

            ShowWindow(mWindowsHandle,SW_SHOW); //show our window
            UpdateWindow(mWindowsHandle); //update our window
            SetForegroundWindow(mWindowsHandle); //set our window on top
            SetFocus(mWindowsHandle); //set the focus on our window

            mWindowsTitle = windowsTitle;
        }

        ~pWinDriver()
        {
            DestroyWindow(mWindowsHandle); //destroy the window
            UnregisterClass("D3DTEST",mInstance); //unregister our window class
        }

        HWND GetWindowsHandle() const
        {
            return mWindowsHandle;
        }
        void SwapBuffer()
        {    MSG msg; //declare a MSG local variable for the GetMessage of the while loop
        if (GetMessage(&msg,NULL,0,0)) //GetMessage reffer to the wndProc() function
        {
            TranslateMessage(&msg); //translate the msg of the GetMessage of your while loop
            DispatchMessage(&msg); //dispath the msg of the GetMessage of your while loop
        }
        //calculate fps
        double currentTime = 1.0;//glfwGetTime();
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
            //glfwSetWindowTitle( fps.str().c_str());
            mFPS=0;
        }
        mPreviousFrameTime = currentTime;

        }

        Real GetDt() const { return mDt; }

    private:
        HINSTANCE mInstance;
        HWND mWindowsHandle;
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