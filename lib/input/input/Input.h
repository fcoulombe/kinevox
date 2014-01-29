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
#include <cstdlib>
#include <stdint.h>
#include <cstdio>

//we put this in the header since people use SDL enum for keys and we don't want them to have to include sdl manually. include input and its all there by default
//#include <3rdparty/Sdl.h>
#include <gcl/Rect.h>

#include <windriver/EventHandler.h>

namespace GCL
{
class Input
{
public:
	static const Rect<int> &ProcessSelection();
	static void ProcessInput();
	static bool IsKeyUp(uint32_t key);
	static bool IsKeyDown(uint32_t key);
	static bool IsLMouseDown();
	static size_t GetMouseX();
	static size_t GetMouseY();
	static void Terminate();
private:

};
}
