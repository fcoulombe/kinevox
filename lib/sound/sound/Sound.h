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
#include <list>
#include <3rdparty/OpenAL.h>
#include <gcl/WorldUnit.h>

namespace GCL
{
class SoundResource;

class Sound
{
public:
	~Sound();
	bool LoadSound(const char *filename);
	bool IsValid() const { return mSoundResource!=NULL; }
	bool IsPlaying() const;
	void Save(const char *filename);
	void Play();
	void Stop();
	void Pause();
	void Rewind();

	Real GetCurrentPlayTime() const;
	Real GetTotalTime() const;
private:
    friend class SoundManager;
	Sound(const char *filename);
	ALuint mBuffer;
	ALuint mSources;

	const SoundResource *mSoundResource;
    
};
typedef std::shared_ptr<Sound> SoundPtr;
typedef std::list<SoundPtr> SoundList;

}

