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
	}
	~BufferWriter()
	{
		delete [] buffer;
	}
	void Pad()
	{
		size_t pad = 4-mCurrentOffset%4;
		//std::cout << "cur: " << mCurrentOffset << "add: " << pad << std::endl;
		mCurrentOffset +=pad;

	}
	template<typename T>
	void Write(T &val)
	{
		//std::cout << "adding: " <<sizeof(T) << " to: " << mCurrentOffset << std::endl;
		memcpy(&(buffer[mCurrentOffset]), &val, sizeof(T));
		mCurrentOffset += sizeof(T);
	}
	void WriteToFile(const char *filename)
	{
		std::ofstream fp(filename, std::ios::out|std::ios::binary);
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
}
