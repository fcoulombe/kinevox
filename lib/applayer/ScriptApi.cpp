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

#include "applayer/ScriptApi.h"
#include <script/ScriptResourceManager.h>
#include <script/Lua.h>

using namespace GCL;

extern int luaopen_objectLib(lua_State * L);
extern int luaopen_kinevoxLib(lua_State * L);
extern int luaopen_inputLib(lua_State * L);
extern int luaopen_soundLib(lua_State * L);
ScriptApi::ScriptApi()
{
	ScriptResourceManager::Instance().ExposeModule("kinevox",luaopen_kinevoxLib);
	ScriptResourceManager::Instance().ExposeObjectModule("object",luaopen_objectLib);
	ScriptResourceManager::Instance().ExposeModule("input",luaopen_inputLib);
	ScriptResourceManager::Instance().ExposeModule("sound",luaopen_soundLib);
}
