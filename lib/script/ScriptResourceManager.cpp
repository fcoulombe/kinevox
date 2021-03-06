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

#include "script/ScriptResourceManager.h"
#include "script/Lua.h"
#include "script/ScriptResource.h"

using namespace GCL;

ScriptResourceManager *ScriptResourceManager::smpInstance = NULL;

ScriptResourceManager::ScriptResourceManager()
{
    mLuaState = new LuaState();
}

ScriptResourceManager::~ScriptResourceManager()
{
    delete mLuaState;
}


Resource * ScriptResourceManager::Allocate( const char *filename )
{
  return new ScriptResource(filename);
}

void ScriptResourceManager::Free( Resource * resource )
{
	delete resource;
}

void ScriptResourceManager::ExposeObjectModule(const char *libName, scriptFunction constructor)
{
    ExposeModule(libName, constructor);
    mObjectModules.push_back(libName);
}
void ScriptResourceManager::ExposeModule(const char *libName, scriptFunction constructor)  const
{
    LuaState &tempState = *mLuaState;
    luaL_requiref(tempState, libName, constructor, 1);
    lua_pop(tempState, 1);
}
void ScriptResourceManager::ExposeFunction(const char *funcName, scriptFunction func) const
{
    const luaL_Reg f[] = {{funcName, func}, {NULL, NULL}};
    lua_pushglobaltable(*mLuaState);
    luaL_setfuncs(*mLuaState, f, 0);
    lua_pop(*mLuaState, 1);
}


void ScriptResourceManager::Update()
{
	mLuaState->RunGarbageCollection();
}