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

#include <common/BufferWriter.h>
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


};
BufferWriter &operator<<(BufferWriter &buffer, const TestData &data)
{
	buffer.Write(data.val1);
	buffer.Write(data.val2);
	buffer.Write(data.val3);
	buffer.Write(data.val4);
	return buffer;
}
void Test()
{
	TEST_START
	BufferWriter buffer(4096);
	TestData data;
	data.val1 = 0;
	data.val2 = 1;
	data.val3 = 2;
	data.val4 = 3;
	buffer << data;
	Assert_Test(buffer.GetCurrentOffset() == sizeof(TestData));

	buffer.WriteToFile("testData.dat");
}
}
