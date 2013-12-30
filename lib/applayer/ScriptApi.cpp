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
#include "applayer/Actor.h"
#include <script/ScriptResourceManager.h>
#include <script/Lua.h>
#include <gcl/Config.h>

using namespace GCL;

long GetObjectId(lua_State * L)
{
	lua_getglobal(L,"KINEVOS_ACTOR_ID");
	long objectid = lua_tointeger(L, lua_gettop(L));
	lua_pop(L, 1);
	return objectid;
}

int KLog(lua_State * L)
{
	const char *str = lua_tostring(L, 1);
	std::cout << "[Script] " << str << std::endl;
	return 1;
}
int KGetPosition(lua_State * L)
{
	long objectid = GetObjectId(L);

	const Actor *actor = (const Actor *)objectid;
	const WorldPoint3 &pos = actor->GetPosition();
	lua_newtable(L);

	lua_pushnumber(L,0);
	lua_pushinteger(L,pos.x);
	lua_settable(L,-3);
	lua_pushnumber(L,1);
	lua_pushinteger(L,pos.y);
	lua_settable(L,-3);
	lua_pushnumber(L,2);
	lua_pushinteger(L,pos.z);
	lua_settable(L,-3);

	return 1;
}

int KGetScreenSize(lua_State * L)
{
	lua_newtable(L);

	int height = Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");
	lua_pushnumber(L,1);
	lua_pushinteger(L,height);
	lua_settable(L,-3);
	int width = Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
	lua_pushnumber(L,0);
	lua_pushinteger(L,width);
	lua_settable(L,-3);
	return 1;
}

static const luaL_Reg kinevoxExposedFunc[] = {
		{ "Log", KLog },
		{ "GetPosition", KGetPosition},
		{ "GetScreenSize", KGetScreenSize},
		{ NULL, NULL } };
int luaopen_kinevoxLib(lua_State * L) {
	luaL_newlib(L, kinevoxExposedFunc);
	return 1;
}

ScriptApi::ScriptApi() {
	ScriptResourceManager::Instance().ExposeModule("kinevox",
			luaopen_kinevoxLib);
}
