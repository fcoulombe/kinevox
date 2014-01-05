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
#include <sstream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <gcl/Assert.h>
namespace GCL
{
    class LuaState
    {
    private:
        lua_State *L;
    public:
        LuaState() 
            : L(luaL_newstate()) 
        { 
			luaL_openlibs(L); 

			//create a table to store the config threads
			lua_newtable(L); //push new table on the stack 1
			lua_setfield(L, LUA_REGISTRYINDEX, "ConfigTable"); //store it in the registry
		}

        ~LuaState() {
			lua_pushnil(L);
			lua_setfield(L, LUA_REGISTRYINDEX, "ConfigTable"); //delete config table
            lua_close(L);
        }

        // implicitly act as a lua_State pointer
        inline operator lua_State*()
        		{
            return L;
        }
        void ReportLuaErrors( const int status)
        {
            if ( status!=0 )
            {
                std::stringstream err;
                err<< std::endl << lua_tostring(L, -1) ;
                lua_pop(L, 1); // remove error message
                GCLAssertMsg(false, err.str().c_str());
            }
        }

		static void StackDump(lua_State *L)
		{
			static int dumpCount = 0;
			std::cout <<  dumpCount++;
			int i;
			int top = lua_gettop(L);
			for (i = 1; i <= top; i++) 
			{  /* repeat for each level */
				int t = lua_type(L, i);
				switch (t)
				{
				case LUA_TSTRING:  /* strings */
					std::cout << "`" <<  lua_tostring(L, i) << "'";
					break;

				case LUA_TBOOLEAN:  /* booleans */
					if (lua_toboolean(L, i)!=0)
						std::cout << "true";
					else
						std::cout << "false";
					break;

				case LUA_TNUMBER:  /* numbers */
					std::cout << lua_tonumber(L, i);
					break;

				default:  /* other values */
					std::cout << lua_typename(L, t);
					break;

				}
				std::cout << "  ";  /* put a separator */
			}
			std::cout << std::endl;  /* end the listing */
		}
        void DumpMyStack () 
        {
           StackDump(L);
        }

		void RunGarbageCollection()
		{
			lua_gc(L, LUA_GCSTEP, 1);
		}
    };
}
