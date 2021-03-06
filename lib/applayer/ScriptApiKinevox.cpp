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
#include "applayer/GameStateManager.h"
#include "applayer/ScriptedGameState.h"
#include "applayer/GCLApplication.h"
#include "applayer/ScriptComponent.h"
#include <gcl/Config.h>
#include <gcl/Log.h>
#include <script/ScriptResource.h>
#include <script/Lua.h>

using namespace GCL;

static int ScriptLog(lua_State * L)
{
	const char *str = lua_tostring(L, 1);
	KLog("[Script] %s", str);
	return 1;
}
static int KLoadLib(lua_State * L)
{
    const char *lib = lua_tostring(L, 1);
    std::stringstream s;
    s<<SCRIPT_PATH<<lib<<".lua";
    //int res = luaL_loadfile(L, s.str().c_str());
    ResourceFile scriptFile(s.str().c_str());
    auto buffer = scriptFile.ReadAll();
    std::string name = std::string("@")+s.str();
    int res = luaL_loadbuffer(L,(const char *)std::get<0>(buffer).get(), std::get<1>(buffer),name.c_str());
    (void)res;
    int ret = lua_pcall(L,0,LUA_MULTRET,0);   
    if ( ret!=0 )
    {
        std::stringstream err;
        const char *errorMsg = lua_tostring(L, -1);
        err<< std::endl << errorMsg;
        lua_pop(L, 1); // remove error message
        GCLAssertMsg(false, err.str().c_str());
    }
    return 1;
}

static int KGetActor(lua_State * L)
{
    const char *actorName = lua_tostring(L, 1);
    Actor *actor = GCLApplication::GetActor(actorName);
    if (actor->HasComponent("ScriptComponent"))
    {
        Component *tempComponent = actor->GetComponent("ScriptComponent");
        ScriptComponent *tempScriptComponent = static_cast<ScriptComponent*>(tempComponent);
        
        lua_getfield(L, LUA_REGISTRYINDEX, tempScriptComponent->GetScriptResource()->GetFileName().c_str());
        lua_pushlightuserdata(L, actor);
        lua_setfield(L,-2,"KINEVOX_ACTOR_ID");
    }
    else
    {
        lua_newtable(L); // 1
        lua_getglobal(L,"_G"); // 21
        lua_setfield(L,-2,"__index"); // 1
        lua_getfield(L, -1, "__index"); // 21
        lua_getfield(L, -1, "object"); // 321
        lua_pushnil(L); // 4321
        while(lua_next(L, -2)) { // load key value at 54321
            const char *functionName = lua_tostring(L, -2);
            lua_setfield(L, -5, functionName);
        }
        lua_pop(L, 1);
        lua_pop(L, 1);
        lua_pushlightuserdata(L, actor);
        lua_setfield(L,-2,"KINEVOX_ACTOR_ID");
    }
    //lua_pushlightuserdata(L, actor);
    return 1;
}

static int KGetScreenSize(lua_State * L)
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

static int KChangeToState(lua_State * L)
{
    const char *stateName = lua_tostring(L, 1);
    const char *scriptFileName = lua_tostring(L, 2);
    GameStateManager::ChangeToState(std::make_shared<ScriptedGameState>(stateName, scriptFileName));

    return 1;
}

static int KCreateActor(lua_State * L)
{
    const char *actorName = lua_tostring(L, 1);
    const char *archetypeFileName = lua_tostring(L, 2);
    GCLApplication::CreateActor(actorName, archetypeFileName);
    return 1;
}

static int KDestroyActor(lua_State * L)
{
    const char *actorName = lua_tostring(L, 1);
    GCLApplication::DestroyActor(actorName);
    return 1;
}


static int KPushChildState(lua_State * L)
{
    const char *stateName = lua_tostring(L, 1);
    const char *scriptFileName = lua_tostring(L, 2);
    GameStateManager::GetCurrentState()->PushChildState(std::make_shared<ScriptedGameState>(stateName, scriptFileName));
    return 1;
}

static int KPushState(lua_State * L)
{
    const char *stateName = lua_tostring(L, 1);
    const char *scriptFileName = lua_tostring(L, 2);
    GameStateManager::PushState(std::make_shared<ScriptedGameState>(stateName, scriptFileName));

    return 1;
}
static int KPopState(lua_State * /*L*/)
{
    GameStateManager::PopState();
    return 1;
}

static const luaL_Reg kinevoxExposedFunc[] = {
		{ "Log", ScriptLog },
		{ "GetScreenSize", KGetScreenSize},
		{ "GetActor", KGetActor},
        { "LoadLib", KLoadLib},
        { "ChangeToState", KChangeToState},
        { "PushChildState", KPushChildState},
        { "PushState", KPushState},
        { "PopState", KPopState},
        { "DestroyActor", KDestroyActor},
        { "CreateActor", KCreateActor},
		{ NULL, NULL } };


int luaopen_kinevoxLib(lua_State * L)
{
	luaL_newlib(L, kinevoxExposedFunc);
	return 1;
}


