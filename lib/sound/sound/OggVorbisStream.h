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

#include <memory>
#include <gcl/Macro.h>

#include <3rdparty/OpenAL.h>
#include <3rdparty/OggVorbis.h>

namespace GCL
{
class OggStream
{
public:
	~OggStream();
	void Open(const char *filename); // obtain a handle to the file
	void Release();         // release the file handle
	void Play();        // play the Ogg stream
	void Stop();
	bool IsPlaying();         // check if the source is playing
	void Update();          // update the stream if necessary

	const vorbis_info* GetVorbisInfo() const { return mVorbisInfo; }
	const vorbis_comment* GetVorbisComment() const { return mVorbisComment; }
private:
    friend class SoundManager;
    OggStream(const char *filename);
    bool IsPlayingUnsafe();         // check if the source is playing
	FILE*           mFp;       // file handle
	OggVorbis_File  mOggStream;     // stream handle
	vorbis_info*    mVorbisInfo;    // some formatting data
	vorbis_comment* mVorbisComment; // user comments

	ALuint mBuffers[2]; // front and back buffers
	ALuint mSource;     // audio source
	ALenum mFormat;     // internal format
	static const size_t BUFFER_SIZE;

	void Stream(ALuint buffer);   // reloads a buffer

	friend std::ostream& operator<<( std::ostream& output, const OggStream &P);
};

typedef std::shared_ptr<OggStream> OggStreamPtr; 


}

