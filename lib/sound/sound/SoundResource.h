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

#include <3rdparty/OpenAL.h>
#include <gcl/Resource.h>
#include "sound/WAVLoaderData.h"

namespace GCL
{
  class SoundResource : public Resource
  {
    public:

    SoundResource(const char *soundName);
    ~SoundResource();

    struct SoundData
    {
      SoundData()
      {
        soundData = NULL;
      }
      ~SoundData()
      {

      }

      WavHeader header;
      uint8_t *soundData;
    }mSoundData;

    static const SoundResource EmptySound;

    static void LoadWav(std::istream &is, SoundData &data);
    static void Unload(SoundData &data);

    ALenum GetFormat() const
    {
    	auto bitsPerSample = GetBitsPerSample();
    	auto numChannels = GetNumChannels();
    	if (bitsPerSample == 8)
    	{
			if (numChannels == 1)
				return AL_FORMAT_MONO8;
			else
				return AL_FORMAT_STEREO8;
    	}
    	else if (bitsPerSample == 16)
    	{
			if (numChannels == 1)
				return AL_FORMAT_MONO16;
			else
				return AL_FORMAT_STEREO16;
    	}
    	else
    	{
    		GCLAssert(false);
    		return -1;
    	}
    }
    uint16_t GetNumChannels() const { return mSoundData.header.channel; }
    uint32_t GetSampleRate() const { return mSoundData.header.sampleRate; }
    uint16_t GetBitsPerSample() const { return mSoundData.header.bitsPerSample; }
  private:
    SoundResource() {}
  };
}
