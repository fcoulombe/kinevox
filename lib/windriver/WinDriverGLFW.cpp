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
#include <windriver/ViewPort.h>

namespace GCL
{

namespace
{
    const size_t KEYS_STATE_ARRAY_SIZE = 350;
    std::map<uint32_t, bool> smKeys;
    bool smIsLMousedown = false;
    size_t smMouseX=0;
    size_t smMouseY=0;
}
void GLFWCALL KeyCallBack(int key,int state)
{
    if (state == GLFW_PRESS)
    {
        smKeys[key] = true;
    }
    else if (state == GLFW_RELEASE)
    {
        smKeys[key] = false;
    }
    else
    {
        GCLAssert(false);
    }
}

void GLFWCALL MouseMoveCallback(int i ,int j)
{

}
void GLFWCALL MouseButtonCallback(int i ,int j)
{

}
void GLFWCALL MouseWheelCallback(int i )
{

}

class pWinDriver
{
public:
    pWinDriver(const char *windowsTitle)
    {
        bool ret = glfwInit(); 
        GCLAssert(ret);

        int width = mViewPort.GetWidth();
        int height = mViewPort.GetHeight();

        ret = glfwOpenWindow( width,height, 0,0,0,0,0,0, GLFW_WINDOW );
        GCLAssert(ret);

        glfwSetWindowTitle( windowsTitle ); 
        glfwSetKeyCallback( KeyCallBack );
        glfwSetMousePosCallback( MouseMoveCallback);
        glfwSetMouseButtonCallback( MouseButtonCallback);
        glfwSetMouseWheelCallback( MouseWheelCallback );

        glViewport(0,0,width,height); glErrorCheck();
    }

    ~pWinDriver()
    {
        glfwTerminate();
    }

    void SwapBuffer()
    {
        glfwSwapBuffers();glErrorCheck();
    }

private:
    ViewPort mViewPort;
};
WinDriver::WinDriver(const char *windowsTitle)
{
    mpWinDriver = new pWinDriver(windowsTitle);
}
WinDriver::~WinDriver()
{
    delete mpWinDriver;
}
void WinDriver::SwapBuffer()
{
    mpWinDriver->SwapBuffer();
}
}
#endif