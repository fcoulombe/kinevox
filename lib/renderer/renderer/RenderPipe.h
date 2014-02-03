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
#include <vector>
#include <gcl/Assert.h>
#include "renderer/RenderThread.h"

#define ENABLE_RENDER_THREAD 1

namespace GCL
{
	class RenderThread;
	class RenderCommand;
	class ReturnMessage;
class RenderPipe
{
public:
	static void Initialize();
	static void Terminate();

	template<class T>
	static void SendCommand(const T &cmd);
	template<class T>
	static void SendCommandSync(const T &cmd);

	//must pass command that actually return a value. otherwise this will stall
	template<class T>
	static const ReturnMessage &SendCommandSyncRet(const T &cmd);
	static void SendReturnMessage(ReturnMessage *retMsg);

private:
	static std::vector<RenderThread *> mRenderThreads;
	static ReturnMessage *mRetMsg;
	static bool mIsInitialized;
};
template<class T>
void GCL::RenderPipe::SendCommand(const T &cmd )
{
	//if threaded, the thread processes the command otherwise it's the pipe itself
	mRenderThreads[0]->SendCommand(cmd);
}
template<class T>
void GCL::RenderPipe::SendCommandSync(const T &cmd )
{
	//if threaded, the thread processes the command otherwise it's the pipe itself
	mRenderThreads[0]->SendCommandSync(cmd);
}
template<class T>
const GCL::ReturnMessage &GCL::RenderPipe::SendCommandSyncRet(const T &cmd )
{
	if (mRetMsg)
	{
		delete mRetMsg;
		mRetMsg = NULL;
	}
	mRenderThreads[0]->SendCommandSync(cmd);
	GCLAssert(mRetMsg);
	return *mRetMsg;
}
}
