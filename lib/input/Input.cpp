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

#include "input/Input.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <gcl/Assert.h>
#include <gcl/Point2.h>
#include <gcl/Rect.h>

#include <windriver/EventHandler.h>

using namespace GCL;


class GeneralInputListener : public InputListener
{
public:
	GeneralInputListener()
        :     smIsMousedown(false),
        smMouseX(0),
        smMouseY(0)
    {

    }
    virtual void KeyDown(uint32_t key)
    {
         smKeys[key] = true;
    }

    virtual void KeyUp(uint32_t key)
    {
         smKeys[key] = false;
    }

    virtual void ClickDown(size_t x, size_t y)
    {
    	smIsMousedown = true;
    	smMouseX = x;
    	smMouseY = y;
    }

    virtual void ClickUp(size_t x, size_t y)
    {
    	smIsMousedown = false;
    	smMouseX = x;
    	smMouseY = y;
    }
    static const size_t KEYS_STATE_ARRAY_SIZE = 350;
    std::map<uint32_t, bool> smKeys;
    bool smIsMousedown;
    size_t smMouseX;
    size_t smMouseY;
};



GeneralInputListener *inputListener = nullptr;

void Input::ProcessInput()
{
}

size_t Input::GetMouseX()
{
    if (!inputListener)
        inputListener = new GeneralInputListener();
    return inputListener->smMouseX;
}
size_t Input::GetMouseY()
{
    if (!inputListener)
        inputListener = new GeneralInputListener();
    return inputListener->smMouseY;
}
bool Input::IsMouseDown()
{
    if (!inputListener)
        inputListener = new GeneralInputListener();
    return inputListener->smIsMousedown;
}
bool Input::IsKeyUp(uint32_t key)
{
    if (!inputListener)
        inputListener = new GeneralInputListener();
    return inputListener->smKeys[key];
}
bool Input::IsMouseUp()
{
    if (!inputListener)
        inputListener = new GeneralInputListener();
    return inputListener->smIsMousedown;
}
bool Input::IsKeyDown(uint32_t key)
{
	if (!inputListener)
		inputListener = new GeneralInputListener();
	return inputListener->smKeys[key];
}

void Input::Terminate()
{
	delete inputListener;
	inputListener = nullptr;
}

