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
#include "windriver/TTFFont.h"

#include <sstream>
#include <3rdparty/Sdl.h>
#include <gcl/Assert.h>
#include <gcl/PixelBuffer.h>
#include "windriver/FontResource.h"
#include "windriver/FontResourceManager.h"


using namespace GCL;

TTFFont::TTFFont(const char *fontName, size_t /*point*/)
{
	const Resource *tempResource = FontResourceManager::Instance().LoadResource(fontName);
	mFontResource = static_cast<const FontResource*>(tempResource);

}

TTFFont::~TTFFont()
{
	FontResourceManager::Instance().ReleaseResource(mFontResource);
}

void TTFFont::BlitText(PixelBuffer &buffer, const char * text, size_t /*x*/, size_t /*y*/)
{
	SDL_Surface* fontSurface;
	SDL_Color fColor;
	fColor.r = 255;
	fColor.b = 255;
	fColor.g = 255;

	fontSurface = TTF_RenderText_Solid(mFontResource->mFont, text, fColor);

	//size_t bufferSize = fontSurface->format->BytesPerPixel*fontSurface->w*fontSurface->h;
	size_t bufferSize = 3*fontSurface->w*fontSurface->h;
	buffer.mPixels = new uint8_t[bufferSize];
	memset(buffer.mPixels, 0, bufferSize);
	//memcpy(buffer.mPixels, fontSurface->pixels, bufferSize);
    GCLAssertMsg(fontSurface->format->BytesPerPixel == 1, "we only support bit field or now");
    uint8_t* ptr = (uint8_t*)(fontSurface->pixels);
    for (size_t i=0; i<fontSurface->w; ++i)
    {
        for (size_t j=0; j<fontSurface->h; ++j)
        {
            size_t invHeight = fontSurface->h-j-1;
            size_t currPixelFt = invHeight*fontSurface->w+i;
            size_t currPixel = j*fontSurface->w+i;
            if (ptr[currPixelFt])
            {
                buffer.mPixels[currPixel*3] = fColor.r;
                buffer.mPixels[currPixel*3+1] = fColor.g;
                buffer.mPixels[currPixel*3+2] = fColor.b;
                //*(SDL_Color*)(&(buffer.mPixels[i*3])) = fColor;
            }
        }
    }
    
   /* size_t ftypeSize =  fontSurface->format->BytesPerPixel*fontSurface->w*fontSurface->h;
	for (int i=0; i<ftypeSize; ++i)
	{
		uint8_t* ptr = (uint8_t*)(fontSurface->pixels);
		if (ptr[ftypeSize - i])
        {
            buffer.mPixels[i*3] = fColor.r;
            buffer.mPixels[i*3+1] = fColor.g;
            buffer.mPixels[i*3+2] = fColor.b;
			//*(SDL_Color*)(&(buffer.mPixels[i*3])) = fColor;
        }
	}*/
	buffer.mHeight = fontSurface->h;
	buffer.mWidth = fontSurface->w;
	buffer.mBitsPerPixel = 3*8;//fontSurface->format->BytesPerPixel*8;
	buffer.mBytesPerPixel = 3;//fontSurface->format->BytesPerPixel;
    buffer.mBitDepth = 8;
	SDL_FreeSurface(fontSurface);
}
#endif