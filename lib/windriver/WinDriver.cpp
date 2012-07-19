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

#include "windriver/WinDriver.h"

#include <sstream>
#include <3rdparty/Sdl.h>
#include <3rdparty/OpenGL.h>

#include <gcl/Assert.h>

using namespace GCL;

WinDriver::WinDriver()
{
    int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
    GCLAssert(sdlInitSuccessful>= 0);

    const SDL_VideoInfo* info = NULL;
    int width = mViewPort.GetWidth();
    int height = mViewPort.GetHeight();
#ifdef ES1
    int flags = SDL_OPENGL | SDL_WINDOW_SHOWN;
#else
    int flags = SDL_OPENGL;
#endif

#if 0 //defined(OS_IPHONE)
    SDL_Window *window;
    SDL_Renderer *r;
    /* create window and renderer */
    window =
        SDL_CreateWindow(NULL, 0, 0, width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }

    r = SDL_CreateRenderer(window, -1, 0);
    if (!r) {
        printf("Could not create renderer\n");
        return 1;
    }

#else
    info = SDL_GetVideoInfo( );

    if( !info )
    {
        fprintf( stderr, "Video query failed: %s\n",
            SDL_GetError( ) );
        return;
    }
    int bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    //std::cout << "GLRenderer::Init3DState> w " << width << " h " <<  height << " bpp " << bpp << " f " << flags ;
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 )
    {
        std::stringstream s;
        s<<"Video mode set failed: " <<	SDL_GetError( ) << std::endl;
        GCLAssertMsg(false,  s.str().c_str());
        return;
    }
#endif

    glViewport(0,0,width,height); glErrorCheck();
}
WinDriver::~WinDriver()
{
    SDL_Quit();
}
void WinDriver::SwapBuffer()
{
    SDL_GL_SwapBuffers();glErrorCheck();
}

