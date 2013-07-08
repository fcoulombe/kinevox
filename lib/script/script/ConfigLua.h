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

#include <memory>
#include <gcl/WorldUnit.h>
struct lua_State;
namespace GCL
{
	class ScriptResource;
	class LuaTableIterator
	{
	public:
		enum ConfigType
		{
			CT_BOOL,
			CT_STRING,
			CT_NUMBER,
			CT_TABLE,
			CT_EMPTY
		};

		void operator++();
		bool End() const { return mIsAtEnd; }
		bool GetBool() const;
		int GetInt() const;
		Real GetReal() const;
		const std::string GetString() const;
		std::unique_ptr<LuaTableIterator> GetTableIterator() const;
		ConfigType GetType() const { return mType; }
	private: 
		void operator=(LuaTableIterator &);
		LuaTableIterator();
		LuaTableIterator(LuaTableIterator &);
		LuaTableIterator(const std::string &tablePath, lua_State *configState, const char *resourceKey);

		void Next();
		size_t mStackSize;
		bool mIsAtEnd;
		ConfigType mType;
		lua_State *mL;
		lua_State *mConfigState;
		std::string mTablePath;
		const char *mResourceKey;
		friend class ConfigLua;
	};

	typedef std::unique_ptr<LuaTableIterator> PtrLuaTableIterator;
	class ConfigLua
	{
	public:
		ConfigLua(const char *configLua);
		~ConfigLua();
		int GetInt(const char *key) const;
		int GetInt(const char *key, size_t tableIndex) const;

		Real GetReal(const char *key) const;

		const std::string GetString(const char *key) const;
		PtrLuaTableIterator GetTableIterator(const std::string &key);

	private:
		const ScriptResource *mResource;
		LuaTableIterator *mIterator;
		lua_State *L;
	};
}
