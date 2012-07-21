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
void TTFFont::DrawText(PixelBuffer &buffer, const char * text, size_t /*x*/, size_t /*y*/)
{
	SDL_Surface* fontSurface;
	SDL_Color fColor;
	fColor.r = 255;
	fColor.b = 255;
	fColor.g = 255;

	fontSurface = TTF_RenderText_Solid(mFontResource->mFont, text, fColor);

	size_t bufferSize = fontSurface->format->BytesPerPixel*fontSurface->w*fontSurface->h;
	buffer.mPixels = new uint8_t[bufferSize];
	memcpy(buffer.mPixels, fontSurface->pixels, bufferSize);
	buffer.mHeight = fontSurface->h;
	buffer.mWidth = fontSurface->w;
	buffer.mBitsPerPixel = fontSurface->format->BytesPerPixel*8;
	buffer.mBytesPerPixel = fontSurface->format->BytesPerPixel;

	SDL_FreeSurface(fontSurface);
}
