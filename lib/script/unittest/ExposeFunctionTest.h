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
#include <sstream>

#include <kinetestlib/UnitTest.h>
#include <script/Lua.h>
#include <script/ScriptResourceManager.h>

using namespace GCL;
namespace ExposeFunctionTest
{
    class TestScriptedObject
    {
    public:
    };
    TestScriptedObject object;
    void FunctionToExpose(int bleh, int blah, const char *str);
    void FunctionToExpose(int bleh, int blah, const char *str)
    {
        std::cout << "exposed function: " << bleh << " " << blah << " " << str<<std::endl;
    }

    int FunctionToExposeProx(lua_State * /*L*/)
    {
        std::cout << "we call a C func!" << std::endl;
        return 1;
    }

    static const luaL_Reg kinevoxExposedFunc [] = {
        {"FunctionToExpose", FunctionToExposeProx},
        {NULL, NULL}
    };
    int luaopen_kinevoxLib(lua_State * L )
    {
        luaL_newlib (L, kinevoxExposedFunc);
        return 1;
    }

    static size_t GetObjectId(lua_State * L)
    {
        int top = lua_gettop(L);
        lua_getfield(L, -top, "KINEVOX_ACTOR_ID");
        size_t objectid = (size_t)lua_topointer(L, -1);
        lua_pop(L, 1);
        return objectid;
    }
    int ObjectFunctionToExposeProx(lua_State * L)
    {
        size_t objectid = GetObjectId(L);
        size_t testObjectId = (size_t)&object;
        GCLAssert(objectid == testObjectId);
        std::cout << "we call an object func!" << std::endl;
        return 1;
    }

    static const luaL_Reg objectExposedFunc [] = {
        {"ObjectFunctionToExpose", ObjectFunctionToExposeProx},
        {NULL, NULL}
    };
    int luaopen_objectLib(lua_State * L )
    {
        luaL_newlib (L, objectExposedFunc);
        return 1;
    }

    void Test()
    {
        KINEVOX_TEST_START
        ScriptResourceManager::Initialize();

        ScriptResourceManager::Instance().ExposeObjectModule("scripted_object", luaopen_objectLib);
        ScriptResourceManager::Instance().ExposeModule("kinevox", luaopen_kinevoxLib);
        ScriptResourceManager::Instance().ExposeFunction("FunctionToExpose", FunctionToExposeProx);
        {
            
            const Resource *resource = ScriptResourceManager::Instance().LoadResource(SCRIPT_PATH"ExposeToLuaTest.lua");
            const ScriptResource *luaResource = static_cast<const ScriptResource*>(resource);

            luaResource->ExecuteMethod("Logic", &object);
            ScriptResourceManager::Instance().ReleaseResource(resource);
			ScriptResourceManager::Instance().Update(); //gc
        }

        ScriptResourceManager::Terminate();
    }
}
