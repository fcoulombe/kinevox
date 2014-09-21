/*
 * Copyright (C) 2014 by Francois Coulombe
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

#include <gcl/WorkerThread.h>
#include "sound/Sound.h"
#include "sound/OggVorbisStream.h"

namespace GCL
{
class SoundManager
{
public:
	static void Initialize();
	static void Terminate();
	static SoundPtr LoadSound(const char *filename);
    static OggStreamPtr LoadMusic(const char *filename);
    static void Update();

    static const char *GetDeviceSpecifier();

    template<class T>
    static void SendCommand(const T &cmd);
    template<class T>
    static void SendCommandSync(const T &cmd);
    static bool IsThreaded()  { return mIsThreaded; }
private:
    static WorkerThread *mCommandQueue;
	static SoundList mSoundList;
    static bool mIsThreaded;
    friend class Sound;
};

template<class T>
void GCL::SoundManager::SendCommand(const T &cmd )
{
    if (IsThreaded())
    {
        mCommandQueue->SendCommandAsync(cmd);
    }
    else
    {
        cmd();
    }
}
template<class T>
void GCL::SoundManager::SendCommandSync(const T &cmd )
{
    if (IsThreaded())
    {
        mCommandQueue->SendCommandSync(cmd);
    }
    else
    {
        cmd();
    }
}

}

