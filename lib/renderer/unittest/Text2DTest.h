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

#include <renderer/Text2D.h>
#include <gcl/PixelBuffer.h>
#include <gcl/Time.h>
#include <gcl/UnitTest.h>
#include <windriver/FontResourceManager.h>
#include <windriver/TTFFont.h>

using namespace GCL;

namespace Text2DTest
{

class TestText2D : public Text2D
{
public:
	TestText2D(const char *text)
	:mFont(FONT_PATH"FreeMono.ttf", 12)
	{
		PixelBuffer buffer;
		mFont.DrawText(buffer, text, 100, 100);
		mTexture = new Texture(buffer);
		mTexture->Save("Text2DTest.tga");
	}

private:
	TTFFont mFont;
};
void Test()
{
	TEST_START

	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	{
		std::stringstream s;
		WinDriver winDriver;
		GLRenderer renderer;

		TestText2D obj("HelloWorld!");
		s.str("");
		s<< obj.GetWidth()<<" == 118"<<std::endl;
		AssertMsg_Test(obj.GetWidth() == 118, s.str().c_str());
		s.str("");
		s<< obj.GetHeight()<<" == 19"<<std::endl;
		AssertMsg_Test(obj.GetHeight() == 19, s.str().c_str());

		static const WorldPoint3 kPositionTest(320.0, 240.0, 0.0);
		obj.SetPosition(kPositionTest);
		Assert_Test(obj.GetPosition() == kPositionTest);

		const WorldPoint2 kScaleTest(0.5, 0.5);
		obj.SetScale(kScaleTest);
		Assert_Test(obj.GetScale() == kScaleTest);


		for (size_t i=0;i<100; ++i)
		{
			obj.Update();
			renderer.PreRender();
			glPushMatrix();glErrorCheck();
			obj.Render();
			glPopMatrix();glErrorCheck();
			renderer.PostRender();
			winDriver.SwapBuffer();
			Time::SleepMs(1);
		}
	}
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
}
