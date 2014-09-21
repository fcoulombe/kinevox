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

#include "sound/OggVorbisStream.h"
#include <iostream>
#include <gcl/File.h>
#include "sound/SoundManager.h"

using namespace GCL;

const size_t OggStream::BUFFER_SIZE = (4096 * 8);

OggStream::OggStream(const char *filename)
{
	Open(filename);
}
OggStream::~OggStream()
{
	Stop();
    SoundManager::SendCommandSync([=]{
	alDeleteSources(1, &mSource);	alErrorCheck();
	alDeleteBuffers(2, mBuffers);	alErrorCheck();

	ov_clear(&mOggStream);
    });
}

void OggStream::Open(const char *filename)
{
    SoundManager::SendCommand([=]{
	int result;

	GCLAssert(GCLFile::Exists(filename));
	result = ov_fopen(filename, &mOggStream); ovErrorCheck(result);

	mVorbisInfo = ov_info(&mOggStream, -1);
	mVorbisComment = ov_comment(&mOggStream, -1);

	if(mVorbisInfo->channels == 1)
		mFormat = AL_FORMAT_MONO16;
	else
		mFormat = AL_FORMAT_STEREO16;

	alGenBuffers(2, mBuffers);	alErrorCheck();
	alGenSources(1, &mSource);	alErrorCheck();

	alSource3f(mSource, AL_POSITION,        0.0, 0.0, 0.0);alErrorCheck();
	alSource3f(mSource, AL_VELOCITY,        0.0, 0.0, 0.0);alErrorCheck();
	alSource3f(mSource, AL_DIRECTION,       0.0, 0.0, 0.0);alErrorCheck();
	alSourcef (mSource, AL_ROLLOFF_FACTOR,  0.0          );alErrorCheck();
	alSourcei (mSource, AL_SOURCE_RELATIVE, AL_TRUE      );alErrorCheck();
    });
}


std::ostream& operator<<( std::ostream& output, const OggStream &P)
{
	const vorbis_info* vorbisInfo =  P.GetVorbisInfo();
	output 	<< "version         " << vorbisInfo->version         << std::endl
			<< "channels        " << vorbisInfo->channels        << std::endl
			<< "rate (hz)       " << vorbisInfo->rate            << std::endl
			<< "bitrate upper   " << vorbisInfo->bitrate_upper   << std::endl
			<< "bitrate nominal " << vorbisInfo->bitrate_nominal << std::endl
			<< "bitrate lower   " << vorbisInfo->bitrate_lower   << std::endl
			<< "bitrate window  " << vorbisInfo->bitrate_window  << std::endl
			<< "\n";

	const vorbis_comment* vorbisComment =  P.GetVorbisComment();
	output		<< "vendor " << vorbisComment->vendor << std::endl;

	for(int i = 0; i < vorbisComment->comments; i++)
		output << "   " << vorbisComment->user_comments[i] << std::endl;

	return output;
}



void OggStream::Play()
{
    SoundManager::SendCommand([&]{
	if(IsPlayingUnsafe())
		return;

	Stream(mBuffers[0]);

	Stream(mBuffers[1]);

	alSourceQueueBuffers(mSource, 2, mBuffers);alErrorCheck();
	alSourcePlay(mSource);alErrorCheck();
    });

}

void OggStream::Stop()
{
    SoundManager::SendCommand([&]{
	alSourceStop(mSource);alErrorCheck();
	int queued;

	alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);alErrorCheck();

	while(queued--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(mSource, 1, &buffer);		alErrorCheck();
	}

	//send file back to beginning
	int result = ov_raw_seek(&mOggStream,0);
	ovErrorCheck(result);
    });
}

bool OggStream::IsPlaying()
{
    bool ret;
    SoundManager::SendCommandSync([&]{
        ret = IsPlayingUnsafe();
    });

    return ret;
}

bool OggStream::IsPlayingUnsafe()
{
    ALenum state;
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);alErrorCheck();
    return (state == AL_PLAYING);
}

void OggStream::Update()
{
    SoundManager::SendCommand([&]{
	int processed;
    
	alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);alErrorCheck();
    std::cout << mSource << " " << processed << std::endl;
	while(processed--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers(mSource, 1, &buffer);		alErrorCheck();

		Stream(buffer);

		alSourceQueueBuffers(mSource, 1, &buffer);		alErrorCheck();
	}
    });
}

void OggStream::Stream(ALuint buffer)
{
	char data[BUFFER_SIZE];
	ALsizei  size = 0;
	int  section;
	long  result;

	while(size < (ALsizei)BUFFER_SIZE)
	{
		result = ov_read(&mOggStream, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);
		ovErrorCheck(result);
		if(result > 0)
			size += result;
		else
        {
			ov_time_seek(&mOggStream,0.f);
        }
	}

	GCLAssert(size != 0);

	alBufferData(buffer, mFormat, data, size, mVorbisInfo->rate);	alErrorCheck();
}


