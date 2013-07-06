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


#include "renderer/RenderPipe.h"
#include "renderer/RenderCmd.h"
#include "renderer/GL/GLRenderThread.h"
using namespace GCL;

bool RenderPipe::mIsInitialized = false;
std::vector<RenderThread *> RenderPipe::mRenderThreads;
ReturnMessage *RenderPipe::mRetMsg=NULL;

void GCL::RenderPipe::SendCommand( RenderCommand *cmd )
{
	//if threaded, the thread processes the command otherwise it's the pipe itself
	mRenderThreads[0]->SendCommand(cmd);
}

void GCL::RenderPipe::Render( /*const RenderObject2DList &spriteList */)
{

	/*
	//send the commands to the render thread in multhreaded
	for (size_t i=0; i<spriteList.size(); ++i)
	{

	}
	*/
	SendCommand(new RenderCommand(SWAP_BUFFER));
	//call process commands in single threaded
}

const GCL::ReturnMessage &GCL::RenderPipe::SendCommandSyncRet( RenderCommand *cmd )
{
	if (mRetMsg)
	{
		delete mRetMsg;
		mRetMsg = NULL;
	}
	mRenderThreads[0]->SendCommand(cmd);
	while (!mRetMsg)
		Thread::YieldThread();
	return *mRetMsg;
}

void GCL::RenderPipe::SendReturnMessage( ReturnMessage *retMsg )
{
	mRetMsg = retMsg;
}

void GCL::RenderPipe::Initialize()
{
	GCLAssert(!mIsInitialized);
	mIsInitialized = true;
	mRetMsg = NULL;
	mRenderThreads.push_back(new GLRenderThread());
	if (mRenderThreads[0]->IsThreaded())
		mRenderThreads[0]->Start();
}

void GCL::RenderPipe::Terminate()
{
	GCLAssert(mIsInitialized);

	delete mRenderThreads[0];
	mRenderThreads.clear();
	mIsInitialized = false;
}

