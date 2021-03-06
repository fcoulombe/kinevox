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
#include <script/ScriptResource.h>

using namespace GCL;
namespace ScriptResourceTest
{
    void Test()
    {
        KINEVOX_TEST_START
        ScriptResourceManager::Initialize();
        {
            const Resource *resource = ScriptResourceManager::Instance().LoadResource(SCRIPT_PATH"TestLuaScript.lua");
            const ScriptResource *luaResource = static_cast<const ScriptResource*>(resource);
            luaResource->ExecuteFunction("Logic");

            const Resource *resource2 = ScriptResourceManager::Instance().LoadResource(SCRIPT_PATH"TestLuaScript.lua");
            const ScriptResource *luaResource2 = static_cast<const ScriptResource*>(resource2);
            luaResource2->ExecuteFunction("Logic");

            luaResource2->ExecuteFunction("Logic2", 1.0);


            ScriptResourceManager::Instance().ReleaseResource(resource);
            ScriptResourceManager::Instance().ReleaseResource(resource2);
			ScriptResourceManager::Instance().Update(); //gc
        }
        ScriptResourceManager::Terminate();
    }
}
