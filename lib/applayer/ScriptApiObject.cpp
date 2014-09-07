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
#include "applayer/Actor.h"
#include "applayer/GCLApplication.h"
#include <script/ScriptResourceManager.h>
#include <script/Lua.h>
#include <input/Input.h>

using namespace GCL;


static size_t GetObjectId(lua_State * L)
{
    int top = lua_gettop(L);
    lua_getfield(L, -top, "KINEVOX_ACTOR_ID");
    size_t objectid = (size_t)lua_topointer(L, -1);
    lua_pop(L, 1);
	return objectid;
}

static int KGetName(lua_State * L)
{
    size_t objectid = GetObjectId(L);
    const Actor *actor = (const Actor *)objectid;
    lua_pushstring(L, actor->GetName().c_str());
    return 1;
}

static int KGetPosition(lua_State * L)
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
static int KSetPosition(lua_State * L)
{
	size_t objectid = GetObjectId(L);

	Actor *actor = (Actor *)objectid;

	Real x = lua_tonumber(L, 2);
	Real y = lua_tonumber(L, 3);
	Real z = lua_tonumber(L, 4);
	actor->SetPosition(x, y, z);
	return 1;
}


static const luaL_Reg objectExposedFunc[] = {
		{ "GetPosition", KGetPosition},
		{ "SetPosition", KSetPosition},
        { "GetName", KGetName},
        { NULL, NULL } };

int luaopen_objectLib(lua_State * L)
{
	luaL_newlib(L, objectExposedFunc);
	return 1;
}

