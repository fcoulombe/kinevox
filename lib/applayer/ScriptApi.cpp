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
#include <gcl/Config.h>
#include <script/ScriptResourceManager.h>
#include <script/Lua.h>
#include <input/Input.h>

using namespace GCL;

int KIsKeyUp(lua_State * L)
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
int KIsKeyDown(lua_State * L)
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

size_t GetObjectId(lua_State * L)
{
	lua_getglobal(L,"KINEVOS_ACTOR_ID");
	size_t objectid = lua_tointeger(L, lua_gettop(L));
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
	size_t objectid = GetObjectId(L);

	const Actor *actor = (const Actor *)objectid;
	const WorldPoint3 &pos = actor->GetPosition();
	lua_newtable(L);

	lua_pushinteger(L,0);
	lua_pushnumber(L,pos.x);
	lua_settable(L,-3);
	lua_pushinteger(L,1);
	lua_pushnumber(L,pos.y);
	lua_settable(L,-3);
	lua_pushinteger(L,2);
	lua_pushnumber(L,pos.z);
	lua_settable(L,-3);

	return 1;
}
int KSetPosition(lua_State * L)
{
	size_t objectid = GetObjectId(L);

	Actor *actor = (Actor *)objectid;

	Real x = lua_tonumber(L, 1);
	Real y = lua_tonumber(L, 2);
	Real z = lua_tonumber(L, 3);
	actor->SetPosition(x, y, z);


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
		{ "GetScreenSize", KGetScreenSize},
		{ NULL, NULL } };

static const luaL_Reg objectExposedFunc[] = {
		{ "GetPosition", KGetPosition},
		{ "SetPosition", KSetPosition},
		{ NULL, NULL } };

static const luaL_Reg inputExposedFunc[] = {
	{ "IsKeyDown", KIsKeyDown},
	{ "IsKeyUp", KIsKeyUp},
	{ NULL, NULL } };
int luaopen_kinevoxLib(lua_State * L)
{
	luaL_newlib(L, kinevoxExposedFunc);
	return 1;
}
int luaopen_objectLib(lua_State * L)
{
	luaL_newlib(L, objectExposedFunc);
	return 1;
}
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



ScriptApi::ScriptApi()
{
	ScriptResourceManager::Instance().ExposeModule("kinevox",luaopen_kinevoxLib);
	ScriptResourceManager::Instance().ExposeModule("object",luaopen_objectLib);
	ScriptResourceManager::Instance().ExposeModule("input",luaopen_inputLib);

}
