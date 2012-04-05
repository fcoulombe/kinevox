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
#include <fstream>
#include <sstream>
#include <cstring>

#include <common/BufferWriter.h>
#include <gcl/File.h>
#include <gcl/UnitTest.h>

using namespace GCL;
namespace BufferWriterTest
{

struct TestData
{
	uint32_t val1;
	uint32_t val2;
	uint16_t val3;
	uint16_t val4;
	WorldPoint2 p2;
	WorldPoint3 p3;
	WorldPoint4 p4;
};
BufferWriter &operator<<(BufferWriter &buffer, const TestData &data)
{
	buffer.Write(data.val1);
	buffer.Write(data.val2);
	buffer.Write(data.val3);
	buffer.Write(data.val4);
	buffer << data.p2;
	buffer << data.p3;
	buffer << data.p4;
	return buffer;
}
void Test()
{
	TEST_START
	std::stringstream s;

	BufferWriter buffer(4096);
	TestData data;
	data.val1 = 0;
	data.val2 = 1;
	data.val3 = 2;
	data.val4 = 3;
	data.p2.x = 1.5;
	data.p2.y = 1.7;
	data.p3.x = 1.5;
	data.p3.y = 1.7;
	data.p3.z = 1.0;
	data.p4.x = 1.5;
	data.p4.y = 1.7;
	data.p4.z = 1.0;
	data.p4.w = 0.5;

	buffer << data;
	Assert_Test(buffer.GetCurrentOffset() == sizeof(TestData));
	Assert_Test(memcmp(buffer.GetBuffer(), (const char*)&data,sizeof(TestData))==0);

	//test padding
	{
		uint8_t pad = 8;
		buffer.Write(pad);
		Assert_Test(buffer.GetCurrentOffset() == sizeof(TestData)+sizeof(uint8_t));
		buffer.Pad();
		s<<buffer.GetCurrentOffset() << " == " << sizeof(TestData)+sizeof(uint32_t) << std::endl;
		AssertMsg_Test(buffer.GetCurrentOffset() == sizeof(TestData)+sizeof(uint32_t), s.str().c_str());
	}

	buffer.WriteToFile("testData.dat");
	GCLFile fp("testData.dat");
	Assert_Test(fp.GetFileSize());
}
}
