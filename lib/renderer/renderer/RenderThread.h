/*
 * Copyright (C) 2013 by Francois Coulombe
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
#include <iostream>
#include <queue>
#include <gcl/Assert.h>
#include <gcl/Thread.h>
#include <gcl/Mutex.h>
#include <gcl/Semaphore.h>
#include "renderer/RenderCmd.h"
namespace GCL
{
struct RenderData;
class RenderThread : public Thread
{
public:
	RenderThread();
	virtual ~RenderThread();
	void SendCommand(RenderCommand *cmd);

	//returns false if we disbale multithreading
	bool IsThreaded() const { return mIsThreaded; }
private:
	void Run();
	std::queue<RenderCommand*> mCommandList;
	
	Mutex mMutex;
protected:
	Semaphore mRunMutex;
	RenderCommandFunction *mRenderCommandMap; //vtable for render commands. configured by child class
	RenderData *mRenderData; //holds client side render data
	bool mIsThreaded;
};


}
