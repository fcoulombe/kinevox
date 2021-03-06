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

#include "sound/Sound.h"

#include <cstdlib>
#include <iostream>

#include <3rdparty/OpenAL.h>
#include <gcl/Assert.h>
#include <gcl/Macro.h>
#include "sound/SoundManager.h"
#include "sound/SoundResource.h"
#include "sound/SoundResourceManager.h"

using namespace GCL;

GCLINLINE std::ostream& operator<<( std::ostream& output, const SoundResource &P)
{
	output << "(dataSize: " << P.mSoundData.header.data_size
			<< ", sampleRate: " << P.mSoundData.header.sampleRate
			<< ", format: " << P.mSoundData.header.channel<< ")" << std::endl;
	return output;
}


Sound::Sound(const char *filename)
{
	bool ret = LoadSound(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Sound: ") + std::string(filename)).c_str());
    SoundManager::SendCommand([&]{
	//std::cout << *mSoundResource << std::endl;
	alGenBuffers(1, &mBuffer);alErrorCheck();
	alBufferData(mBuffer,
			mSoundResource->GetFormat(),
			mSoundResource->mSoundData.soundData,
			mSoundResource->mSoundData.header.data_size,
			mSoundResource->GetSampleRate());alErrorCheck();

	alGenSources(1, &mSources);alErrorCheck();
	alSourcef(mSources, AL_PITCH, 1);alErrorCheck();
	alSourcef(mSources, AL_GAIN, 1);alErrorCheck();
	alSource3f(mSources, AL_POSITION, 0, 0, 0);alErrorCheck();
	alSource3f(mSources, AL_VELOCITY, 0, 0, 0);alErrorCheck();
	alSourcei(mSources, AL_LOOPING, AL_FALSE);alErrorCheck();


	alListener3f(AL_POSITION, 0, 0, 0);alErrorCheck();
	alListener3f(AL_VELOCITY, 0, 0, 0);alErrorCheck();
	//alListener3f(AL_ORIENTATION, 0, 0, -1);alErrorCheck();
	alSourcei(mSources, AL_BUFFER, mBuffer);
    });
}
Sound::~Sound()
{
	ALuint buffer = mBuffer;
	ALuint sources = mSources;
    SoundManager::SendCommand([=]{
	alDeleteSources(1, &sources);
	alDeleteBuffers(1, &buffer);
    });
	SoundResourceManager::Instance().ReleaseResource(mSoundResource);
}
bool Sound::LoadSound(const char *filename)
{
	const Resource *tempResource = SoundResourceManager::Instance().LoadResource(filename);
	mSoundResource = static_cast<const SoundResource*>(tempResource);
	return mSoundResource != 0;
}

void Sound::Play()
{
    SoundManager::SendCommand([=]{
	alSourcePlay(mSources);alErrorCheck();
    });
}

void Sound::Stop()
{
    SoundManager::SendCommand([=]{
	alSourceStop( mSources);alErrorCheck();
    });
}
void Sound::Rewind()
{
    SoundManager::SendCommand([=]{
	alSourceRewind( mSources);alErrorCheck();
    });
}
void Sound::Pause()
{
    SoundManager::SendCommand([=]{
	alSourcePause( mSources);alErrorCheck();
    });
}

bool Sound::IsPlaying() const
{
    ALint val;
    SoundManager::SendCommandSync([&]{
	alGetSourcei(mSources,AL_SOURCE_STATE,&val);alErrorCheck();
    });
	return val == AL_PLAYING;
}

Real Sound::GetCurrentPlayTime() const
{
	int byteoffset;
    SoundManager::SendCommandSync([&]{
	alGetSourcei(mSources, AL_BYTE_OFFSET, &byteoffset);
    });
	return Real(byteoffset) / mSoundResource->mSoundData.header.byteRate ;
}

Real Sound::GetTotalTime() const { return Real(mSoundResource->mSoundData.header.data_size)/ mSoundResource->mSoundData.header.byteRate ; }
