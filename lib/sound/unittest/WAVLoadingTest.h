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

#include <3rdparty/OpenAL.h>
#include <gcl/UnitTest.h>
#include <sound/SoundResource.h>
#include <sound/WAVLoaderData.h>

using namespace GCL;
namespace WAVLoadingTest
{
static inline ALenum GetFormatFromInfo(uint16_t channels)
{
	if (channels == 1)
		return AL_FORMAT_MONO16;
	return AL_FORMAT_STEREO16;
}
void Test()
{
	TEST_START
	const char *fullFileName = SOUND_PATH"Explosion.wav";
	std::fstream fp(fullFileName, std::fstream::binary|std::fstream::in);
	AssertMsg_Test( fp.good(), fullFileName);

	SoundResource::SoundData data;
	SoundResource::LoadWav(fp, data);
	fp.close();
	//Assert_Test(data.imageData);
	//Assert_Test(data.mBitdepth==8);
	//Assert_Test(data.mBytePerPixel==4);
	//Assert_Test(data.mWidth==512);
	//Assert_Test(data.mHeight==512);
	SoundResource::Unload(data);

}
}

