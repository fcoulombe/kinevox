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

#ifdef USE_SDL
#include "windriver/WinDriver.h"

#include <sstream>
#include <map>
#include <3rdparty/Sdl.h>
#include <3rdparty/OpenGL.h>

#include <gcl/Assert.h>

#include "windriver/ViewPort.h"



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

    class pWinDriver
    {
    public:
        pWinDriver(const char *windowsTitle)
        {
            int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
            GCLAssert(sdlInitSuccessful>= 0);
            SDL_WM_SetCaption(windowsTitle, NULL);

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
            mScreen = SDL_SetVideoMode( width, height, bpp, flags ) ;
            if(!mScreen)
            {
                std::stringstream s;
                s<<"Video mode set failed: " <<	SDL_GetError( ) << std::endl;
                GCLAssertMsg(false,  s.str().c_str());
                return;
            }
#endif


        }

        ~pWinDriver()
        {
            SDL_Quit();
        }
        void pWinDriver::SwapBuffer()
        {
            SDL_GL_SwapBuffers();glErrorCheck();
            SDL_Event event;

	while ( SDL_PollEvent(&event) )
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			smKeys[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			//printf("%d %s\n",event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));
			smKeys[event.key.keysym.sym] = false;

			break;
		case SDL_MOUSEMOTION:
			/*printf("Mouse moved by %d,%d to (%d,%d)\n",
						event.motion.xrel, event.motion.yrel,
						event.motion.x, event.motion.y);*/
			smMouseX = event.button.x;
			smMouseY = event.button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			/*printf("Mouse button %d pressed at (%d,%d)\n",
						event.button.button, event.button.x, event.button.y);*/
			smIsLMousedown = true;
			smMouseX = event.button.x;
			smMouseY = event.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			/*printf("Mouse button %d pressed at (%d,%d)\n",
						event.button.button, event.button.x, event.button.y);*/
			smIsLMousedown = false;
			smMouseX = event.button.x;
			smMouseY = event.button.y;

			break;
		case SDL_QUIT:
			exit(0);
		}
	}
        }
    private: 
        SDL_Surface *mScreen;
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