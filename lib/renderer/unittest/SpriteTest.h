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

#include <renderer/Sprite.h>
#include <gcl/Time.h>
#include <kinetestlib/UnitTest.h>
#include <input/Input.h>

using namespace GCL;
namespace SpriteTest
{
void Test()
{
	KINEVOX_TEST_START

	TextureResourceManager::Initialize();
	{
		WinDriver winDriver("SpriteTest");
		GLRenderer renderer;

		Sprite obj;
		Assert_Test(obj.GetWidth() == 64);

		Assert_Test(obj.GetHeight() == 64);
		Assert_Test(obj.GetFrameCount() == 128);

		static const WorldPoint3 kPositionTest(320.0, 240.0, 0.0);
		obj.SetPosition(kPositionTest);
		Assert_Test(obj.GetPosition() == kPositionTest);

		const WorldPoint2 kScaleTest(0.5, 0.5);
		obj.SetScale(kScaleTest);
		Assert_Test(obj.GetScale() == kScaleTest);


		Assert_Test(obj.IsPlaying() == false);
		obj.Play();

		Assert_Test(obj.IsPlaying() == true);
		KINEVOX_TEST_LOOP_START
            const ViewPort &viewport = winDriver.GetViewPort();
#if ENABLE_FIX_PIPELINE
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (0, viewport.GetHeight(), viewport.GetHeight(), 0, -1.0f, 1.0f); glErrorCheck();
#endif

#if ENABLE_FIX_PIPELINE
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
#endif
			obj.Update();
			renderer.PreRender();
			glPushMatrix();glErrorCheck();
			obj.Render();
			glPopMatrix();glErrorCheck();
			renderer.PostRender();
			winDriver.SwapBuffer();
			Time::SleepMs(1);
		KINEVOX_TEST_LOOP_END

		obj.Pause();

		Assert_Test(obj.IsPlaying() == false);
		obj.Rewind();
	}
	TextureResourceManager::Terminate();

}
}
