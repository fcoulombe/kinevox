/*
* Copyright (C) 2011 by Francois Coulombe
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
#include <script/Script.h>
#include <script/Lua.h>


using namespace GCL;
namespace LuaTest
{
    LuaState L;

    class Script
    {
    public:
        Script(const char *filename)
            : mFilename(filename)
        {
            int s = luaL_loadfile(L, filename); //1
            L.ReportLuaErrors(s);
            GCLAssertMsg(s==0, filename);


            //create environment
            lua_newtable(L); //push new table on the stack 21
            lua_getglobal(L,"_G"); //put the global on the stack 321
            lua_setfield(L,-2,"__index"); //store it in the table at key index 21

            lua_pushvalue(L,-1); //pushes a copy of the table onto the stack 321
            lua_setfield(L, LUA_REGISTRYINDEX, mFilename.c_str()); //store the table into the registry and pops it 21

            lua_pushvalue(L,-1); //pushes a copy of the table onto the stack 321
            lua_setmetatable(L,-2); //21
            lua_setupvalue(L,1,1); //1
            lua_pcall(L,0,LUA_MULTRET,0);    
        }

        void Logic()
        {
            L.DumpMyStack();
            /* retrieve the environment from the resgistry */
             lua_getfield(L, LUA_REGISTRYINDEX, mFilename.c_str());
             //stackDump(L);
            /* get the desired function from the environment */
            lua_getfield(L, -1, "Logic");
            //stackDump(L);
            int s = lua_pcall(L, 0, 0, 0);     
            L.ReportLuaErrors(s);
        }
    private:
        std::string mFilename;
    };
    class GObj
    {
    public:
        GObj(const char *scriptName = SCRIPT_PATH"TestLuaScript.luac")
            : mScript(scriptName)
        {
            
        }

        void Logic()
        {
            mScript.Logic();
        }
    protected:
        Script mScript;
    };

    void Test()
    {
        KINEVOX_TEST_START
            std::stringstream s;
        {
            GObj obj(SCRIPT_PATH"TestLuaScript.luac");
            GObj obj2(SCRIPT_PATH"TestLuaScript2.luac");
            obj2.Logic();
            obj.Logic();
            

        }
    }
}
