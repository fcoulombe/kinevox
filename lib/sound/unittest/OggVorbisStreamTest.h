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

#include <sound/OggVorbisStream.h>
#include <sound/SoundManager.h>


using namespace GCL;
namespace OggVorbisStreamTest
{
void Test()
{
	TEST_START

	SoundResourceManager::Initialize();
    SoundManager::Initialize();
    {

	OggStreamPtr stream = SoundManager::LoadMusic(MUSIC_PATH"ExampleMusic.ogg");


	stream->Play();
	size_t i=0;
	const size_t kLoopCount = 10; //10000000;
	while (i<kLoopCount) //(stream.IsPlaying())
	{
		stream->Update();
		++i;
	}
	stream->Stop();
	stream->Play();
	i=0;
	while (i<kLoopCount) //(stream.IsPlaying())
	{
		stream->Update();
		++i;
	}
    }
    SoundManager::Terminate();
	SoundResourceManager::Terminate();

}
}
