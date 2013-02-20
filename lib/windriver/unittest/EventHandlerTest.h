/*
 * Copyright (C) 2013 by Francois Coulombe
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

#include <kinetestlib/UnitTest.h>

#include <windriver/EventHandler.h>
#include <windriver/WinDriver.h>

using namespace GCL;

#define SIMULATE_KEYPRESS(key) EventManager::Instance().KeyDown(key);
#define SIMULATE_KEYRELEASE(key) EventManager::Instance().KeyUp(key);

namespace EventHandlerTest
{
    class MyEventHandler : public KeyListener
    {
    public:
        MyEventHandler()
            : hasReceivedKeyDown(false),
            hasReceivedKeyUp(false)
        {}
        virtual void KeyDown(uint32_t key)
        {
            hasReceivedKeyDown = true;
        }

        virtual void KeyUp(uint32_t key)
        {
            hasReceivedKeyUp = true;
        }

        bool hasReceivedKeyDown;
        bool hasReceivedKeyUp;
    };

void Test()
{
	TEST_START

	WinDriver windows("EventHandlerTest");
    MyEventHandler handler;

    KINEVOX_TEST_LOOP_START


    windows.SwapBuffer();
    SIMULATE_KEYPRESS('a');
    SIMULATE_KEYRELEASE('a');
    //SIMULATE_KEYPRESS(KEY_UP);
    //SIMULATE_LEFT_MOUSE_CLICK(128, 128);
    KINEVOX_TEST_LOOP_END
    Assert_Test(handler.hasReceivedKeyDown == true);
    Assert_Test(handler.hasReceivedKeyUp == true);

}
}
