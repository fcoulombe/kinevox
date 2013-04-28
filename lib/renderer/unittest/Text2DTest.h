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
#include <kinetestlib/UnitTest.h>
#include <windriver/FontResourceManager.h>
#include <windriver/TTFFont.h>

using namespace GCL;

namespace Text2DTest
{

class TestText2D : public Text2D
{
public:
	TestText2D(const char *text)
	:mFont(FONT_PATH"FreeMono.ttf")
	{
		PixelBuffer buffer;
		mFont.BlitText(buffer, text, 12, 100, 100);
        //buffer.PadToNextPOT();
        //buffer.SaveTga("Text2DTest.tga", buffer.mWidth, buffer.mHeight, buffer.mBytesPerPixel, buffer.mPixels);
		mTexture = new Texture(buffer);
		mTexture->Save("Text2DTest.tga");
	}

private:
	TTFFont mFont;
};
void Test()
{
	KINEVOX_TEST_START

	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	{
		std::stringstream s;
		WinDriver winDriver("Text2dTest");
		Renderer renderer(winDriver.GetWindowsHandle());

		TestText2D obj("HelloWorld");
		s.str("");
		s<< obj.GetWidth()<<" == 72"<<std::endl;
		AssertMsg_Test(obj.GetWidth() == 72, s.str().c_str());
		s.str("");
		s<< obj.GetHeight()<<" == 12"<<std::endl;
		AssertMsg_Test(obj.GetHeight() == 12, s.str().c_str());

		static const WorldPoint3 kPositionTest(256.0, 256.0, 0.0);
		obj.SetPosition(kPositionTest);
		Assert_Test(obj.GetPosition() == kPositionTest);

		const WorldPoint2 kScaleTest(0.5, 0.5);
		obj.SetScale(kScaleTest);
		Assert_Test(obj.GetScale() == kScaleTest);


        KINEVOX_TEST_LOOP_START
        const ViewPort &viewport = renderer.GetViewPort();
        Matrix44 ortho;
        ortho.SetOrtho(0.0, (Real)viewport.GetHeight(), (Real)viewport.GetWidth(), 0.0, -1.0, 1.0);            
#ifdef ENABLE_FIX_PIPELINE
        renderer.SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY);
#else
        Shader shader;
        shader.Bind();
        renderer.SetTransform(proj, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);
#endif
        obj.Update();
        renderer.PreRender();
        //glPushMatrix();glErrorCheck();
        obj.Render();
        //glPopMatrix();glErrorCheck();
        renderer.PostRender();
        winDriver.SwapBuffer();
        KINEVOX_TEST_LOOP_END

	}
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
}
