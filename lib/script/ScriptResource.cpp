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

#include "script/ScriptResource.h"
#include "script/ScriptResourceManager.h"
#include "script/Lua.h"
#include <gcl/Assert.h>
#include <gcl/File.h>
#include <gcl/Log.h>


using namespace GCL;

ScriptResource::ScriptResource( const char *scriptFileName )
: Resource(),
mFilename(scriptFileName)
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState();
    LuaState tempL(lua_newthread(L)); //1 thread
    GCLFile scriptFile(scriptFileName);
    size_t fileSize = scriptFile.GetFileSize();
    uint8_t *buffer = new uint8_t[fileSize];
    scriptFile.Read(buffer, fileSize);

    std::string name = std::string("@")+scriptFileName;
    int s = luaL_loadbuffer(tempL,(const char*)buffer,fileSize,name.c_str());
    tempL.ReportLuaErrors(s);

    GCLAssertMsg(s==0, scriptFileName);
    //create environment
    lua_newtable(tempL); //push new table on the stack 21
    lua_getglobal(tempL,"_G"); //put the global on the stack 321
    lua_setfield(tempL,-2,"__index"); //store it in the table at key index 21

    lua_pushvalue(tempL,-1); //pushes a copy of the table onto the stack 321

    auto objectModuleList = ScriptResourceManager::Instance().GetObjectModuleList();
    for (auto objectModule : objectModuleList)
    {
        lua_getfield(tempL, -1, "__index"); // load index at 4321
        lua_getfield(tempL, -1, objectModule.c_str()); // load object at 54321
        lua_pushnil(tempL); // load nill at 654321
        while(lua_next(tempL, -2)) { // load key value at 7654321
            const char *functionName = lua_tostring(tempL, -2);
            lua_setfield(tempL, -5, functionName);
        }
        lua_pop(tempL, 1);
        lua_pop(tempL, 1);
    }
    lua_setfield(tempL, LUA_REGISTRYINDEX, mFilename.c_str()); //store the table into the registry and pops it 21

    lua_pushvalue(tempL,-1); //pushes a copy of the table onto the stack 321
    lua_setmetatable(tempL,-2); //21
    lua_setupvalue(tempL,1,1); //1
    int ret = lua_pcall(tempL,0,LUA_MULTRET,0);   
    tempL.ReportLuaErrors(ret);
    lua_pop(L, 1);        
}

ScriptResource::~ScriptResource()
{
    LuaState &L = ScriptResourceManager::Instance().GetLuaState(); 
    //hopefully this is how you unset this
    lua_pushnil(L);
    lua_setfield(L, LUA_REGISTRYINDEX, mFilename.c_str());
}



