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

#include <string>
#include <gcl/Resource.h>
#include "script/Lua.h"
#include "script/ScriptResourceManager.h"

namespace GCL
{
class ScriptResource : public Resource
{
public:
	ScriptResource(const char *scriptFileName);
	~ScriptResource();
    inline void ExecuteFunction(const char *functionName) const;

    inline void ExecuteMethod(const char *functionName, void *obj) const;

    template<typename T>
    inline void ExecuteFunction(const char *functionName, T arg1) const;

    template<typename T>
    inline void ExecuteMethod(const char *functionName, void *obj, T arg1) const;

    const std::string &GetFileName() const { return mFilename; }
private:
	ScriptResource() {}
    std::string mFilename;

};

void ScriptResource::ExecuteFunction(const char *functionName) const
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState();
    LuaState tempL(lua_newthread(L)); //1 thread
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str()); 
    lua_getfield(tempL, -1, functionName);
    GCLAssertMsg(!lua_isnil(tempL, lua_gettop(L)), mFilename + ":" + functionName);
    int s = lua_pcall(tempL, 0, 0, 0);
    tempL.ReportLuaErrors(s);
    lua_pop(tempL, 1);
    lua_pop(L, 1);
}

void ScriptResource::ExecuteMethod(const char *functionName, void *obj) const
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState();
    LuaState tempL(lua_newthread(L)); //1 thread
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str()); 
    lua_getfield(tempL, -1, functionName);
    GCLAssertMsg(!lua_isnil(tempL, lua_gettop(L)), mFilename + ":" + functionName);

    // push self table on stack so that we can use this in an object oriented manner.
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str());
    // store the actor id in the script variable so that we can use it.
    lua_pushlightuserdata(tempL, obj);
    lua_setfield(tempL,-2,"KINEVOX_ACTOR_ID");

    int s = lua_pcall(tempL, 1, 0, 0);
    tempL.ReportLuaErrors(s);

    lua_pop(tempL, 1);
    lua_pop(L, 1);
}

template<typename T>
void ScriptResource::ExecuteFunction(const char *functionName, T arg1) const
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState();
    LuaState tempL(lua_newthread(L)); //1 thread
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str()); 
    lua_getfield(tempL, -1, functionName);
    GCLAssertMsg(!lua_isnil(tempL, lua_gettop(L)), mFilename + ":" + functionName);
    tempL.Push(arg1);
    int s = lua_pcall(tempL, 1, 0, 0);
    tempL.ReportLuaErrors(s);
    lua_pop(tempL, 1);
    lua_pop(L, 1);
}
template<typename T>
void ScriptResource::ExecuteMethod(const char *functionName, void *obj, T arg1) const
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState();
    LuaState tempL(lua_newthread(L)); //1 thread
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str()); 
    lua_getfield(tempL, -1, functionName);
    GCLAssertMsg(!lua_isnil(tempL, lua_gettop(L)), mFilename + ":" + functionName);

    // push self table on stack so that we can use this in an object oriented manner.
    lua_getfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str());
    // store the actor id in the script variable so that we can use it.
    lua_pushlightuserdata(tempL, obj);
    lua_setfield(tempL,-2,"KINEVOX_ACTOR_ID");
    tempL.Push(arg1);

    int s = lua_pcall(tempL, 2, 0, 0);
    tempL.ReportLuaErrors(s);
    lua_pop(tempL, 1);
    lua_pop(L, 1);
}
}
