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

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

using namespace GCL;
namespace LuaTest
{

    class LuaState
    {
    private:
        lua_State *L;
    public:
        LuaState() : L(luaL_newstate()) { luaL_openlibs(L); }

        ~LuaState() {
            lua_close(L);
        }

        // implicitly act as a lua_State pointer
        inline operator lua_State*() {
            return L;
        }
    };

    class ScriptManager
    {
    public:
        ScriptManager()
        {
            luaL_openlibs(L);
        }
        void report_errors(lua_State *L, const int status)
        {
            if ( status!=0 ) {
                std::cerr << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1); // remove error message
            }
        }
        static ScriptManager &Instance() 
        {
            static ScriptManager instance;
            return instance;
        }
        void LoadFile(const char *filename)
        {
            /*            lua_newtable(L);
            lua_newtable(L);
            lua_getglobal(L,"_G");
            lua_setfield(L,-2,"__index");
            lua_pushvalue(L,-1);
            lua_setmetatable(L,-3);
            */
            int s = luaL_loadfile(L, filename);
            GCLAssert(s==0);

            //create environment for the script
            /*    lua_createtable(L, 0, 1);
            lua_getglobal(L, "print");
            lua_setfield(L, -2, "print");
            lua_pushvalue(L, -1);
            lua_setfield(L, LUA_REGISTRYINDEX, filename);


            lua_setuservalue (L, -2);
            lua_call(L, 0, 0);*/

            s = lua_pcall(L, 0, LUA_MULTRET, 0); // must run the script to create the functions
            report_errors(L, s);
            // Push the function name onto the stack

        }
        void RunScript()
        {
            int s;            
            lua_getglobal(L, "Logic");
            s=lua_pcall (L, 0, 0, 0);
            report_errors(L, s);
        }
    private:
        LuaState L;
    };
    LuaState L;
    void report_errors(lua_State *L, const int status)
    {
        if ( status!=0 ) {
            std::cerr << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1); // remove error message
        }
    }
    static void stackDump (lua_State *L) 
    {
        static int dumpCount = 0;
        printf("%d", dumpCount++);
        int i;
        int top = lua_gettop(L);
        for (i = 1; i <= top; i++) {  /* repeat for each level */
            int t = lua_type(L, i);
            switch (t) {

            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;

            default:  /* other values */
                printf("%s", lua_typename(L, t));
                break;

            }
            printf("  ");  /* put a separator */
        }
        printf("\n");  /* end the listing */
    }
    class Script
    {
    public:
        Script(const char *filename)
            : mFilename(filename)
        {
            int s = luaL_loadfile(L, filename); //1
            GCLAssert(s==0);


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
            stackDump(L);
            /* retrieve the environment from the resgistry */
             lua_getfield(L, LUA_REGISTRYINDEX, mFilename.c_str());
             //stackDump(L);
            /* get the desired function from the environment */
            lua_getfield(L, -1, "Logic");
            //stackDump(L);
            int s = lua_pcall(L, 0, 0, 0);     
            report_errors(L, s);
        }
    private:
        std::string mFilename;
    };
    class GObj
    {
    public:
        GObj(const char *scriptName = SCRIPT_PATH"TestLuaScript.luascript")
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
            GObj obj(SCRIPT_PATH"TestLuaScript.luascript");
            GObj obj2(SCRIPT_PATH"TestLuaScript2.luascript");
            obj2.Logic();
            obj.Logic();
            

        }
    }
}
