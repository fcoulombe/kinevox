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
#include <sstream>

#include <gcl/Macro.h>
#include <gcl/UnitTest.h>
#include <sound/ALSoundDevice.h>
#include <sound/SoundResource.h>
#include <sound/SoundResourceManager.h>

using namespace GCL;
namespace SoundResourceTest
{

void Test()
{
	TEST_START

	SoundResourceManager::Initialize();
	{
	
	SoundResourceManager &soundResourceManager = SoundResourceManager::Instance();
	//mono
	{
	const Resource *wavResource;
	wavResource = soundResourceManager.LoadResource(SOUND_PATH"Explosion.wav");
	Assert_Test(wavResource);
	const SoundResource *soundResource = static_cast<const SoundResource*>(wavResource);
	Assert_Test(soundResource->GetNumChannels() == 1);
	Assert_Test(soundResource->GetSampleRate() == 8000);
	Assert_Test(soundResource->GetBitsPerSample() == 8);
	soundResourceManager.ReleaseResource(wavResource);
	}
	//stereo
	{
	const Resource *wavResource;
	wavResource = soundResourceManager.LoadResource(SOUND_PATH"Explosion2.wav");
	Assert_Test(wavResource);
	const SoundResource *soundResource = static_cast<const SoundResource*>(wavResource);
	Assert_Test(soundResource->GetNumChannels() == 1);
	Assert_Test(soundResource->GetSampleRate() == 48000);
	Assert_Test(soundResource->GetBitsPerSample() == 16);
	soundResourceManager.ReleaseResource(wavResource);
	}
	}
	SoundResourceManager::Terminate();

}
}
