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
#include <vector>
#include <gcl/StringUtil.h>
#include "script/ConfigLua.h"
#include "script/Lua.h"
#include "script/ScriptResource.h"

#include "script/ScriptResourceManager.h"

using namespace GCL;
ConfigLua::ConfigLua(const char *configLua)
{
    mResource = static_cast<const ScriptResource *>(ScriptResourceManager::Instance().LoadResource(configLua));
}


void ConfigLua::GetValue(const char* key) const
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState(); 
    lua_getfield(L, LUA_REGISTRYINDEX, mResource->GetFileName().c_str());
    lua_getfield(L, -1, "config");

    std::vector<std::string> tableList = StringUtil::Explode(key, '.');
    size_t lastElement = tableList.size()-1;
    for (size_t i=0; i<lastElement; ++i)
    {
        lua_pushstring(L, tableList[i].c_str());
        lua_gettable(L, -2);
    }
    lua_pushstring(L, tableList[lastElement].c_str());
    lua_gettable(L, -2);
}

int ConfigLua::GetInt(const char *key) const
{
    GetValue(key);
    LuaState &L = ScriptResourceManager::Instance().GetLuaState(); 
    return lua_tointeger(L, -1);  
}

float ConfigLua::GetFloat(const char *key) const
{
    GetValue(key);
    LuaState &L = ScriptResourceManager::Instance().GetLuaState(); 
    return lua_tonumber(L, -1);  
}

const char * ConfigLua::GetString(const char *key) const
{
    GetValue(key);
    LuaState &L = ScriptResourceManager::Instance().GetLuaState(); 
    return lua_tostring(L, -1);  
}

ConfigLua::~ConfigLua()
{
    ScriptResourceManager::Instance().ReleaseResource(mResource);
}
