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
	LuaState &gL = ScriptResourceManager::Instance().GetLuaState(); 

	lua_getfield(gL, LUA_REGISTRYINDEX, "ConfigTable"); //1 get the config table on tthe stack
	L = lua_newthread(gL); //12 thread
	lua_setfield(gL,-2,mResource->GetFileName().c_str()); //1 store the thread in the table at filename
	lua_pop(gL, 1); //remove the table from the stack

	//put the script on our own stack
	lua_getfield(L, LUA_REGISTRYINDEX, mResource->GetFileName().c_str());
}
ConfigLua::~ConfigLua()
{
	LuaState &gL = ScriptResourceManager::Instance().GetLuaState(); 
	lua_getfield(gL, LUA_REGISTRYINDEX, "ConfigTable"); //1 get the config table on tthe stack
	lua_pushnil(gL); //21 add null
	lua_setfield(gL,-2,mResource->GetFileName().c_str()); //1store null where the was a ref to the thread
	lua_pop(gL, 1); //remove the table from the stack
	ScriptResourceManager::Instance().ReleaseResource(mResource);
}

//run through the string and breka it up for every . so that we can make a pseudo path out of tables
void GetValue(const std::string  &key, lua_State *L) 
{
	std::vector<std::string> tableList;
	StringUtil::Explode(key, tableList, '.');
	for (size_t i=0; i<tableList.size(); ++i)
	{
		lua_getfield(L, -1, tableList[i].c_str());
	}
}

int ConfigLua::GetInt(const char *key) const
{
	int top = lua_gettop(L);
	GetValue(key, L);
	int ret = lua_tointeger(L, -1); 
	lua_pop(L, lua_gettop(L)-top);
	return ret;  
}


Real ConfigLua::GetReal(const char *key) const
{
	int top = lua_gettop(L);
	GetValue(key, L);

	Real ret = lua_tonumber(L, -1);  
	lua_pop(L, lua_gettop(L)-top);
	return ret;
}

const std::string ConfigLua::GetString(const char *key) const
{
	int top = lua_gettop(L);
	GetValue(key, L);

	const std::string ret( lua_tostring(L, -1));  
	lua_pop(L, lua_gettop(L)-top);
	return ret;
}



std::unique_ptr<GCL::LuaTableIterator> GCL::ConfigLua::GetTableIterator( const std::string &key ) 
{
	return PtrLuaTableIterator(new GCL::LuaTableIterator(key, L, mResource->GetFileName().c_str()));
}


std::unique_ptr<LuaTableIterator> GCL::LuaTableIterator::GetTableIterator() const
{
	const std::string tableName( lua_tostring(mL, -2));
	return std::unique_ptr<GCL::LuaTableIterator>(new LuaTableIterator(mTablePath+std::string(".")+tableName, mConfigState, mResourceKey));
}

void GCL::LuaTableIterator::operator++()
{
	lua_pop(mL, mStackSize);
	Next();
}

int GCL::LuaTableIterator::GetInt() const
{
	return lua_tointeger(mL, -1);
}

bool GCL::LuaTableIterator::GetBool() const
{
	return (lua_toboolean(mL, -1)==0) ? false : true;
}

GCL::Real GCL::LuaTableIterator::GetReal() const
{
	return lua_tonumber(mL, -1);
}

const std::string GCL::LuaTableIterator::GetString() const
{
	return std::string( lua_tostring(mL, -1));
}

void GCL::LuaTableIterator::Next()
{
	int top = lua_gettop(mL);
	mIsAtEnd = (lua_next(mL, -2)==0) ? true : false;
	int top2 = lua_gettop(mL);
	mStackSize = top2-top;
	if (!End())
	{
		int type = lua_type(mL, -mStackSize);
		switch (type)
		{
		case LUA_TSTRING:
			mType = CT_STRING;
			break;
		case LUA_TBOOLEAN:
			mType = CT_BOOL;
			break;
		case LUA_TNUMBER:
			mType = CT_NUMBER;
			break;
		case LUA_TTABLE:
			mType = CT_TABLE;
			break;
		default:
			GCLAssert(false && "Unsupported Type");
			mType = CT_EMPTY;
		}
	}
	else
	{
		mType = CT_EMPTY;
	}
}
GCL::LuaTableIterator::LuaTableIterator(const std::string &tablePath,  lua_State *configState, const char *resourceKey )
{
	mResourceKey = resourceKey;
	mTablePath = tablePath;
	mConfigState = configState;
	mL = lua_newthread (mConfigState); //we want our own stack to not affect other iterators or config
	lua_getfield(mL, LUA_REGISTRYINDEX, mResourceKey);
	GetValue(mTablePath, mL);
	//setup the iterator
	lua_pushnil(mL);
	Next();
}
