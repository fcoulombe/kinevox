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
        ConfigLua config("data/Script/ConfigLua.luascript");
   
        int testInt = config.GetInt("TestInt");
        Assert_Test(testInt == 666);

        float testFloat = config.GetFloat("TestFloat");
        Assert_Test(abseq(testFloat, 66.66f));

        const char* testString = config.GetString("TestString");
        Assert_Test(strcmp(testString, "TestString") ==0);

        //test accessing embeded members into table
        int testInt2 = config.GetInt("TableConf.tableVal");
        std::cout << testInt2;
        Assert_Test(testInt2 == 456);
        }
        ScriptResourceManager::Terminate();
    }
}
