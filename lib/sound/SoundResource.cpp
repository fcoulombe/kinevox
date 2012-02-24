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

#include "sound/SoundResource.h"


#include <fstream>
#include <iostream>
#include <cstring>

#include "gcl/Assert.h"

using namespace GCL;

const SoundResource SoundResource::EmptySound;

void SoundResource::Unload(SoundData &data)
{
	GCLAssert(data.soundData);
	delete [] data.soundData;
}


void SoundResource::LoadWav(std::istream& fp, SoundData & data)
{
	fp.read((char*)&data.header, sizeof(WavHeader));
	data.soundData = new uint8_t[data.header.data_size];
	fp.read((char *)data.soundData, data.header.data_size);
}


//this loads WAV files and then upload it to openal
SoundResource::SoundResource( const char *soundName )
: Resource()
{
	std::string path = Resource::GetResourcePath();
	path += soundName;

	std::string msg("trying to loads " );
	msg += path.c_str();
	msg += "\n";

	const char *ext = &(path.c_str()[path.length()-3]);
	SoundData &data = mSoundData;
	if (strncmp(ext, "wav", 3) == 0)
	{
		std::fstream fp(path.c_str(), std::fstream::binary|std::fstream::in);
		GCLAssertMsg(fp.is_open() && fp.good(), msg.c_str());

		LoadWav(fp, data);
		GCLAssert(data.soundData);

		fp.close();
	}
	else
	{
		GCLAssertMsg(false, "this extension is not supported")
	}
}

SoundResource::~SoundResource()
{
	if (mSoundData.soundData)
	{
		delete [] mSoundData.soundData;
	}
}

