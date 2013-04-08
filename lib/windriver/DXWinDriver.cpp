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
#ifdef USE_GLFW
#include "windriver/DXWinDriver.h"

#include <sstream>
#include <map>
#include <3rdparty/D3D9.h>

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
            HINSTANCE instance = GetModuleHandle(NULL);
            WNDCLASSEX wc; //window class won't go in detail not related to this tutorial really
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc = (WNDPROC)wndProc;
            wc.cbWndExtra = 0;
            wc.cbClsExtra = 0;
            wc.hInstance = instance;
            wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = NULL;
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
            HWND hWindow = CreateWindowEx(NULL,"D3DTEST","D3D TEST",WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
                0,0,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,instance,NULL);
            if(!hWindow) //if the window failed to create
            {
                DestroyWindow(hWindow); //destroy the window
                UnregisterClass("D3DTEST",instance); //unregister our window class
                GCLAssertMsg(false, "Failed to create the window"); //error pop-up for debug purpose
                
            }
            ShowWindow(hWindow,SW_SHOW); //show our window
            UpdateWindow(hWindow); //update our window
            SetForegroundWindow(hWindow); //set our window on top
            SetFocus(hWindow); //set the focus on our window

            D3DPRESENT_PARAMETERS d3dpp; //the presentation parameters that will be used when we will create the device
            ZeroMemory(&d3dpp,sizeof(d3dpp)); //to be sure d3dpp is empty
            d3dpp.Windowed = 1; //use our global windowed variable to tell if the program is windowed or not
            d3dpp.hDeviceWindow = hWindow; //give the window handle of the window we created above
            d3dpp.BackBufferCount = 1; //set it to only use 1 backbuffer
            d3dpp.BackBufferWidth = width; //set the buffer to our window width
            d3dpp.BackBufferHeight = height; //set the buffer to out window height
            d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; //the backbuffer format
            d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //SwapEffect
            pD3D = Direct3DCreate9(D3D_SDK_VERSION); //Create the presentation parameters
            if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWindow,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&pDevice))) //create the device and check if it failed
            {
                GCLAssertMsg(false, "Failed to create device"); //error pop-up for debug purpose
                if(pD3D) //check if pD3D ism't released
                    pD3D->Release(); //release it
                if(pDevice) //check if pDevice ism't released
                    pDevice->Release(); //release it
            }

            mWindowsTitle = windowsTitle;
            // glfwSetWindowTitle( windowsTitle ); 
            //glfwSetKeyCallback( KeyCallBack );
            //glfwSetMousePosCallback( MouseMoveCallback);
            //glfwSetMouseButtonCallback( MouseButtonCallback);
            //glfwSetMouseWheelCallback( MouseWheelCallback );
        }

        ~pWinDriver()
        {
            if(pD3D) //check if pD3D ism't released
                pD3D->Release(); //release it
            if(pDevice) //check if pDevice ism't released
                pDevice->Release(); //release it
        }

        void SwapBuffer()
        {    MSG msg; //declare a MSG local variable for the GetMessage of the while loop
        if (GetMessage(&msg,NULL,0,0)) //GetMessage reffer to the wndProc() function
        {

            pDevice->Present(NULL,NULL,NULL,NULL); //display your buffer on screen
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
        IDirect3D9 *pD3D; //global variable that will be used by a couple of our function
        IDirect3DDevice9 *pDevice; //a device that will be used for most of our function created inside *pD3D
        //LPD3DXFONT pFont;


        std::string mWindowsTitle;
        double mPreviousFrameTime;
        size_t mFPS;
        double mAccumulatedTime;
        Real mDt;
    };
    DXWinDriver::DXWinDriver(const char *windowsTitle)
    {
        EventManager::Initialize();
        mpWinDriver = new pWinDriver(windowsTitle);
    }
    DXWinDriver::~DXWinDriver()
    {
        delete mpWinDriver;
        EventManager::Terminate();
    }
    void DXWinDriver::SwapBuffer()
    {
        mpWinDriver->SwapBuffer();
    }
    Real DXWinDriver::GetDt() const
    {
        return mpWinDriver->GetDt();
    }
}
#endif