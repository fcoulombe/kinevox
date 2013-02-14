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
#pragma once

#include <gcl/UnitTest.h>
#include <gcl/ResourceManagerConfig.h>
#include <gcl/PixelBuffer.h>
#include <windriver/TTFFont.h>

using namespace GCL;
namespace TTFFontTest
{

void Test()
{
	TEST_START
	FontResourceManager::Initialize();
	{
		TTFFont font(FONT_PATH"verdana.ttf");
#if 0
        for (size_t i=65; i<90; ++i)
        {
        const Glyph &aGlyph = font.GetGlyph((char)i, 18);
        
    /*    char filename[512];
        sprintf(filename, "char/TestTTFontCap%cGlyph.tga", i);
        PixelBuffer::SaveTga(filename, 
                            aGlyph->mWidth, aGlyph->mHeight, 
                            aGlyph->mBytesPerPixel, 
                            aGlyph->mPixels);
        delete aGlyph;*/
        }
        PixelBuffer &buff = font.m_pages[18].texture;
        PixelBuffer::SaveTga("TestTTFFontPageSave.tga", 
            buff.mWidth, buff.mHeight, 
            buff.mBytesPerPixel, 
            buff.mPixels);

#endif
		PixelBuffer buffer;
		font.BlitText(buffer, "Hello World!", 18, 100, 100);
        //font.BlitText(buffer, "ABCD", 18, 100, 100);
		PixelBuffer::SaveTga("TestTTFFont.tga",
							buffer.mWidth, buffer.mHeight,
							buffer.mBytesPerPixel,
							buffer.mPixels);
	/*	buffer.PadToNextPOT();
		PixelBuffer::SaveTga("TestTTFFont2.tga",
									buffer.mWidth, buffer.mHeight,
									buffer.mBytesPerPixel,
									buffer.mPixels);*/

	}
	FontResourceManager::Terminate();
}
}
