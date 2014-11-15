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
#include <script/ConfigLua.h>
#include <script/ScriptResourceManager.h>

using namespace GCL;
namespace ConfigLuaTest
{

    void Test()
    {
        KINEVOX_TEST_START
        ScriptResourceManager::Initialize();
        {
        ConfigLua config("data/Script/ConfigLua.lua");
   
        int testInt = config.GetInt("config.TestInt");
        Assert_Test(testInt == 666);

        Real testFloat = config.GetReal("config.TestFloat");
        Assert_Test(abseq(testFloat, 66.66, DBL_PRECISION_TOLERANCE));

        const std::string testString = config.GetString("config.TestString");
        Assert_Test(testString == "TestString");

        //test accessing embeded members into table
        int testInt2 = config.GetInt("config.TableConf.tableVal");
        Assert_Test(testInt2 == 456);

		PtrLuaTableIterator it = config.GetTableIterator("config.TableIterate");
		while (!it->End())
		{
			switch (it->GetType())
			{
			case LuaTableIterator::CT_BOOL:		
				{
				Assert_Test(it->GetBool() == false);
				break;
				}
			case LuaTableIterator::CT_NUMBER:
				{
					int intret = it->GetInt() ; 
				Assert_Test(intret == 4 || intret == 65);
				Real intret2 = it->GetReal() ; 
				Assert_Test(abseq(intret2, 4.0)  || abseq(intret2, 65.0));
				break;
				}
			case LuaTableIterator::CT_STRING:
				{
					const std::string rets = it->GetString();
				Assert_Test(rets == "some string");
				break;
				}
			case LuaTableIterator::CT_TABLE:
				{
					PtrLuaTableIterator reit = it->GetTableIterator();
				Assert_Test(!reit->End());
				const std::string tableName = it->GetKey();
				Assert_Test(tableName == "SomeTable");
				while (!reit->End())
				{
					++(*reit);
				}
				break;
				}
			default:
				break;
			}
			++(*it);
		}
        }
        ScriptResourceManager::Terminate();
    }
}
