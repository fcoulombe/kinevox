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

#include <sstream>
#include <gcl/UnitTest.h>
#include <gcl/PixelBuffer.h>
#include <renderer/Texture.h>
#include <renderer/TTFFont.h>
#include <renderer/FontResourceManager.h>

using namespace GCL;
namespace TextureTest
{
bool CompareImages(const char * /*filename1*/, const char * /*filename2*/);
bool CompareImages(const char * /*filename1*/, const char * /*filename2*/)
{
	return false;
}
void Test();
void Test()
{
	TEST_START

	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	WinDriver winDriver("TextureTest");
	Renderer renderer(winDriver.GetWindowsHandle());
	std::stringstream s;
	//png
	{
		Texture texture(TEXTURE_PATH"mario_fire.png");
		texture.Bind();
#ifdef ES2
		try
		{
			texture.GetWidth();
			Assert_Test(false);
		}
		catch(...)
		{}
		try
		{
			texture.GetHeight();
			Assert_Test(false);
		}
		catch(...)
		{}
		try
		{
			texture.GetBytesPerPixel();
			Assert_Test(false);
		}
		catch(...)
		{}

#else
		s.str("");
		s<<texture.GetWidth() << " == " << 256 << std::endl;
		AssertMsg_Test(texture.GetWidth() == 256, s.str().c_str());
		s.str("");
		s<<texture.GetHeight() << " == " << 256 << std::endl;
		AssertMsg_Test(texture.GetHeight() == 256, s.str().c_str());
		s.str("");
		s<<texture.GetBytesPerPixel() << " == " << 4 << std::endl;
		AssertMsg_Test(texture.GetBytesPerPixel() == 4, s.str().c_str());
#endif
		
		s.str("");
		s<<texture.GetResourceWidth() << " == " << 256 << std::endl;
		AssertMsg_Test(texture.GetResourceWidth() == 256, s.str().c_str());
		s.str("");
		s<<texture.GetResourceHeight() << " == " << 256 << std::endl;
		AssertMsg_Test(texture.GetResourceHeight() == 256, s.str().c_str());
		s.str("");
		s<<texture.GetResourceBytesPerPixel() << " == " << 4 << std::endl;
		AssertMsg_Test(texture.GetResourceBytesPerPixel() == 4, s.str().c_str());
#ifndef OS_IPHONE
		texture.Save("PNGTextureTest.tga");
#endif

		//Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));

	}

	//rgba
	{
		Texture texture(TEXTURE_PATH"mushroomtga.tga");
		texture.Bind();
		Assert_Test(texture.GetResourceWidth() == 512);
		Assert_Test(texture.GetResourceHeight() == 512);

		texture.Save("RGBATextureTest.tga");
		//Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));
	}

	//rgb
	{
		Texture texture(TEXTURE_PATH"HappyFish.tga");
		texture.Bind();

		texture.Save("RGBTextureTest.tga");
	}

    //rgb
    {
        Texture texture(TEXTURE_PATH"nonsquare.tga");
        texture.Bind();

        texture.Save("NonSquareTextureTest.tga");
    }

	//Pixel Buffer Texture
	{
		TTFFont font(FONT_PATH"FreeMono.ttf");
		PixelBuffer buffer;
		font.BlitText(buffer, "HelloWorld", 18, 0,0);
		//buffer.PadToNextPOT();
		//PixelBuffer::SaveTga("PixelBufferTextureTest.tga", buffer.mWidth, buffer.mHeight, buffer.mBytesPerPixel, buffer.mPixels);
		Texture testTexture(buffer);
		testTexture.Bind();

		testTexture.Save("PixelBufferTextureTest.tga");
	}
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
}
