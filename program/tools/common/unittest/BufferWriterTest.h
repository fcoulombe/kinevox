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
#include <iomanip>

#include <common/BufferWriter.h>
#include <gcl/File.h>
#include <gcl/UnitTest.h>

using namespace GCL;
namespace BufferWriterTest
{

struct TestData
{
	WorldPoint4 p4; //0
	WorldPoint3 p3; //36
	WorldPoint2 p2; //60
	uint32_t val1; //76
	uint32_t val2; //80
	uint16_t val3; //82
	uint16_t val4; //84
};
BufferWriter &operator<<(BufferWriter &buffer, const TestData &data)
{

	buffer << data.p4;
	buffer << data.p3;
	buffer << data.p2;
	buffer.Write(data.val1);
	buffer.Write(data.val2);
	buffer.Write(data.val3);
	buffer.Write(data.val4);
	return buffer;
}
void Test()
{
	TEST_START
	std::stringstream s;

	//test get padding size
	{
		size_t paddingSize = BufferWriter::GetPaddingSize();
#if USE_64BIT_PLATFORM
		Assert_Test(paddingSize == 8);
#else
		Assert_Test(paddingSize == 4);
#endif
	}
	//test stream in
	{
		BufferWriter buffer(4096);
		TestData data;
		memset( &data, 0xfe,sizeof(TestData));
		data.p4.x = 1.5;
		data.p4.y = 1.7;
		data.p4.z = 1.0;
		data.p4.w = 0.5;
		data.p3.x = 1.5;
		data.p3.y = 1.7;
		data.p3.z = 1.0;
		data.p2.x = 1.5;
		data.p2.y = 1.7;
		data.val4 = 3;
		data.val3 = 2;
		data.val2 = 1;
		data.val1 = 666;

		buffer << data;
		buffer.Pad();

		s.str("");
		s<<std::setprecision(16)<< std::endl<<buffer.GetCurrentOffset()<< std::endl <<"==" << std::endl << sizeof(TestData);
		AssertMsg_Test(buffer.GetCurrentOffset() == sizeof(TestData), s.str().c_str());

		for (size_t i=0; i<sizeof(TestData); ++i)
		{
			s.str("");
			s<<std::endl<<"byte: " << i << " " << (int)buffer.GetBuffer()[i]  <<" == " << (int)((const uint8_t*)&data)[i];
			AssertMsg_Test((int)(buffer.GetBuffer()[i]) == (int)(((const uint8_t*)&data)[i]), s.str().c_str());
		}
		Assert_Test(memcmp(buffer.GetBuffer(), (const char*)&data,sizeof(TestData))==0);

		//s.str("");
		//s<<std::setprecision(16)<< std::endl<<GLRenderer::GetGLProjection() << std::endl <<"==" << std::endl << camera.GetProjection();
		Assert_Test(memcmp(buffer.GetBuffer(), (const char*)&data,sizeof(TestData))==0);


		buffer.WriteToFile("testData.dat");
		GCLFile fp("testData.dat");
		Assert_Test(fp.GetFileSize());
	}
	//test padding
	{
		BufferWriter buffer(4096);
		uint8_t pad = 8;
		buffer.Write(pad);
		Assert_Test(buffer.GetCurrentOffset() == sizeof(uint8_t));
		buffer.Pad();
		s<<buffer.GetCurrentOffset() << " == " << BufferWriter::GetPaddingSize() << std::endl;
		AssertMsg_Test(buffer.GetCurrentOffset() == BufferWriter::GetPaddingSize(), s.str().c_str());
	}
	//test padding when no need to
	{
		BufferWriter buffer(4096);
		uint32_t pad = 8;
		buffer.Write(pad);
		Assert_Test(buffer.GetCurrentOffset() == sizeof(uint32_t));
		buffer.Pad();
		s<<buffer.GetCurrentOffset() << " == " << BufferWriter::GetPaddingSize() << std::endl;
		AssertMsg_Test(buffer.GetCurrentOffset() == BufferWriter::GetPaddingSize(), s.str().c_str());
	}

	//string writing test
	{
		BufferWriter buffer(4096);

		const std::string kTestStringValue("myString");
		size_t curOffset = buffer.GetCurrentOffset();
		uint8_t *rawBuffer = buffer.GetBuffer();
		std::string testStr(kTestStringValue);
		buffer << testStr;
		const std::string testStr2((const char*)&(rawBuffer[curOffset+sizeof(uint32_t)]));
		Assert_Test(kTestStringValue == testStr2);
		size_t newOffset = buffer.GetCurrentOffset();

		s.str("");
		s<<newOffset-curOffset-sizeof(uint32_t) << " == " << kTestStringValue.length() <<std::endl;
		AssertMsg_Test(newOffset-curOffset-sizeof(uint32_t) == Memory::Align32(kTestStringValue.length()+1), s.str().c_str());
	}
}
}
