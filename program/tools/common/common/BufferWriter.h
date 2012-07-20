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
#include <iostream>
#include <fstream>
#include <string>

#include <gcl/Assert.h>
#include <gcl/Memory.h>
#include <gcl/Point2.h>
#include <gcl/Point3.h>
#include <gcl/Point4.h>

namespace GCL
{


class BufferWriter
{
public:
	BufferWriter(size_t bufferSize)
	: mCurrentOffset(0),
	  mBufferSize(bufferSize)
	{
		buffer = new uint8_t[mBufferSize];
		memset(buffer, 0xfe, mBufferSize);
	}
	~BufferWriter()
	{
		delete [] buffer;
	}
	void Pad()
	{
#if USE_64BIT_PLATFORM
		mCurrentOffset = Memory::Align64(mCurrentOffset);
#else
		mCurrentOffset = Memory::Align32(mCurrentOffset);
#endif
		//std::cout << "cur: " << mCurrentOffset << "add: " << pad << std::endl;
		GCLAssertMsg(mCurrentOffset <= mBufferSize, "your buffer is too small. please increase its size");
	}

	static size_t GetPaddingSize()
	{
#if USE_64BIT_PLATFORM
		return sizeof(uint64_t);;
#else
		return sizeof(uint32_t);
#endif
	}
	template<typename T>
	void Write(T &val)
	{
		//std::cout << "adding: " <<sizeof(T) << " to: " << mCurrentOffset << std::endl;
		memcpy(&(buffer[mCurrentOffset]), &val, sizeof(T));
		mCurrentOffset += sizeof(T);
		GCLAssertMsg(mCurrentOffset <= mBufferSize, "your buffer is too small. please increase its size");
	}
	//assumes null terminated string
	void Write(const char *str, size_t len)
	{
		memcpy(&(buffer[mCurrentOffset]), str, len+1);
		mCurrentOffset += len+1;
		GCLAssertMsg(mCurrentOffset <= mBufferSize, "your buffer is too small. please increase its size");
	}

	void WriteToFile(const std::string &filename)
	{
		std::fstream fp(filename.c_str(), std::ios::out|std::ios::binary);
		GCLAssert(fp.good());
		//std::cout << "should write: " << mCurrentOffset << std::endl;
		fp.write((const char *)buffer, mCurrentOffset);
		fp.close();
	}
	size_t GetCurrentOffset() const { return mCurrentOffset; }
	uint8_t *GetBuffer() { return buffer; }
private:
	uint8_t *buffer;
	size_t mCurrentOffset;
	size_t mBufferSize;
};

GCLINLINE BufferWriter &operator<<(BufferWriter &buffer, const WorldPoint2 &data)
{
	buffer.Write(data.x);
	buffer.Write(data.y);
	return buffer;
}
GCLINLINE BufferWriter &operator<<(BufferWriter &buffer, const WorldPoint3 &data)
{
	buffer.Write(data.x);
	buffer.Write(data.y);
	buffer.Write(data.z);
	return buffer;
}
GCLINLINE BufferWriter &operator<<(BufferWriter &buffer, const WorldPoint4 &data)
{
	buffer.Write(data.x);
	buffer.Write(data.y);
	buffer.Write(data.z);
	buffer.Write(data.w);
	return buffer;
}
GCLINLINE BufferWriter & operator<<( BufferWriter& buffer, const std::string &stringData)
{
	size_t len = stringData.length();
	uint32_t paddedLen = uint32_t(Memory::Align(len+1));
	buffer.Write(paddedLen);
	buffer.Write(stringData.c_str(), len);
	buffer.Pad();

	return buffer;
}
}
