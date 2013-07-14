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

#include <gcl/UnitTest.h>
#include <gcl/Time.h>
#include <gcl/ThreadManager.h>
#include <windriver/EventHandler.h>

namespace GCL
{

class KinevoxTestCounter : public TestCounter
{
public:
	KinevoxTestCounter(const char *file)
	: TestCounter(file),
    mIsInteractive(gTestConfig.mIsInteractive)
	{

	}

	~KinevoxTestCounter()
	{

	}

	bool mIsInteractive;
};

class KineTestKeyListener : public KeyListener
{
public:
    KineTestKeyListener() : mIsLooping(false) {}
    virtual void KeyDown(uint32_t /*key*/)
    {
    }
    virtual void KeyUp(uint32_t key)
    {
        if (key == GCL_ESCAPE) 
            mIsLooping = false;
    }
    bool mIsLooping;
};

}


#define KINEVOX_TEST_START KinevoxTestCounter testCounter(__FILE__);

//this loop will run at 60fps
#define KINEVOX_TEST_LOOP_START \
    {\
    KineTestKeyListener kineTestKeyListener;\
    size_t i=0;\
	kineTestKeyListener.mIsLooping = testCounter.mIsInteractive;\
	const size_t TICKS_PER_SECOND = 60;\
const size_t SKIP_TICKS = 1000 / TICKS_PER_SECOND;\
const int MAX_FRAMESKIP = 5;\
size_t next_game_tick = GCL::Time::GetTickMs()-1; \
int loops;\
while(i<2 || kineTestKeyListener.mIsLooping)\
{\
			loops = 0;\
			while(GCL::Time::GetTickMs() > next_game_tick && loops < MAX_FRAMESKIP) \
{\
\


#define KINEVOX_TEST_LOOP_END 	\
ThreadManager::ReThrowException();\
	next_game_tick += SKIP_TICKS;\
loops++;\
}\
	++i;\
}\
}\
\


