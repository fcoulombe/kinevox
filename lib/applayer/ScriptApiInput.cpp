/*
 * Copyright (C) 2014 by Francois Coulombe
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
#include <input/Input.h>

using namespace GCL;

static int KIsKeyUp(lua_State * L)
{
	lua_Integer key = lua_tointeger(L, 1);
	if (Input::IsKeyUp((uint32_t)key))
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}
static int KIsKeyDown(lua_State * L)
{
	lua_Integer key = lua_tointeger(L, 1);
	if (Input::IsKeyDown((uint32_t)key))
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}



static const luaL_Reg inputExposedFunc[] = {
	{ "IsKeyDown", KIsKeyDown},
	{ "IsKeyUp", KIsKeyUp},
	{ NULL, NULL } };

#define REGISTER_GLOBAL_KEY(key) 	lua_pushinteger(L, key);lua_setglobal(L,#key);
int luaopen_inputLib(lua_State * L)
{
	luaL_newlib(L, inputExposedFunc);
	REGISTER_GLOBAL_KEY(GCL_ESCAPE);
	REGISTER_GLOBAL_KEY(GCL_UP);
	REGISTER_GLOBAL_KEY(GCL_DOWN);
	REGISTER_GLOBAL_KEY(GCL_LEFT);
	REGISTER_GLOBAL_KEY(GCL_RIGHT);
	//lua_pushinteger(L, GCL_ESCAPE);
	//lua_setglobal(L,"GCL_ESCAPE");
	return 1;
}

