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

#include "sound/OpenAL.h"

#include <gcl/Assert.h>

#include <iostream>
using namespace GCL;

ALCdevice *device=NULL;
ALCcontext *context = NULL;
void Sound::Initialize()
{

	device = alcOpenDevice(NULL);
	GCLAssert(device);
	context=alcCreateContext(device,NULL);
	alcMakeContextCurrent(context);alErrorCheck();


	std::cout << alcGetString(device, ALC_DEVICE_SPECIFIER )<< std::endl;
	alErrorCheck();
	ALuint buffer;
	alGenBuffers(1, &buffer);alErrorCheck();
	//alBufferData();alErrorCheck();

	//alGenSources();alErrorCheck();
	//alSourcei();alErrorCheck();
	//alSourcePlay();alErrorCheck();
}


Sound::Sound(const char *)
{

}
void Sound::Play()
{

}

void Sound::Terminate()
{
	context=alcGetCurrentContext();
	device=alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
