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

#include "sound/SoundManager.h"
#include <iostream>
#include <gcl/Config.h>
#include "sound/ALSoundDevice.h"

using namespace GCL;

static ALSoundDevice *device=nullptr;
static OggStreamPtr currentMusic=nullptr;
bool SoundManager::mIsThreaded = false;
SoundList SoundManager::mSoundList;
WorkerThread *SoundManager::mCommandQueue = nullptr;
void GCL::SoundManager::Initialize()
{
    std::cout << "SoundManager::Initialize" << std::endl;
    GCLAssert(device == nullptr);
    if (Config::Instance().GetInt("IS_RENDERER_THREDED"))
        mIsThreaded = true;
    else
        mIsThreaded = false;
    mCommandQueue = new WorkerThread("SoundThread");
    device = new ALSoundDevice;

}

void GCL::SoundManager::Terminate()
{
    std::cout << "SoundManager::Terminae" << std::endl;
    Update();
    mSoundList.clear();
    GCLAssert(device);
    currentMusic = nullptr;
    delete device;
    device = nullptr;
    delete mCommandQueue;
    mCommandQueue = nullptr;
}

GCL::SoundPtr GCL::SoundManager::LoadSound( const char *filename )
{
    std::cout << "Load Sound: " << filename << std::endl;
    SoundPtr ptr(new Sound(filename));
    mSoundList.push_front(ptr);
    return ptr;
}

const char * GCL::SoundManager::GetDeviceSpecifier()
{
    return device->GetDeviceSpecifier();
}

GCL::OggStreamPtr GCL::SoundManager::LoadMusic( const char *filename )
{
    std::cout << "Load Music: " << filename << std::endl;
    OggStreamPtr ptr(new OggStream(filename));
    currentMusic = ptr;
    return ptr;
}

void GCL::SoundManager::Update()
{
    //std::cout << "SoundManager::Update" << std::endl;
    if (currentMusic)
        currentMusic->Update();
    for (auto it = mSoundList.begin(); it != mSoundList.end(); )
    {
        SoundPtr sound = *it;
        if (!sound->IsPlaying())
        {
            it = mSoundList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
