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

#include <iostream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
namespace GCL
{
    class LuaState
    {
    private:
        lua_State *L;
    public:
        LuaState() 
            : L(luaL_newstate()) 
        { luaL_openlibs(L); }

        ~LuaState() {
            lua_close(L);
        }

        // implicitly act as a lua_State pointer
        inline operator lua_State*() {
            return L;
        }
        void ReportLuaErrors( const int status)
        {
            if ( status!=0 ) {
                std::cerr << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1); // remove error message
            }
        }
        void StackDump () 
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
    };
}
