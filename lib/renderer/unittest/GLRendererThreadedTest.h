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

#include <gcl/Mutex.h>
#include <gcl/Semaphore.h>
#include <gcl/Thread.h>
#include <gcl/ThreadManager.h>
#include <gcl/Time.h>
#include <gcl/Matrix33.h>
#include <kinetestlib/UnitTest.h>
#include <renderer/Renderer.h>
#include <windriver/WinDriver.h>
#include <renderer/RenderPipe.h>
#include <renderer/RenderCmd.h>

using namespace GCL;




namespace GLRendererThreadedTest
{ 
#if 0
	class ThreadedVertexBuffer
	{

	};
	class ThreadedTexture
	{

	};
	class ThreadedShader
	{

	};
	class ThreadedVAO
	{

	};
	class ThreadedRenderObject2D
	{
	public:
		Matrix33 mTransform;
	};
	class ThreadedSprite : public ThreadedRenderObject2D
	{
	public:

	};
	class MyRenderObject2D : public RenderObject2D
	{
	public:
		MyRenderObject2D()
			: RenderObject2D("TestObject", "DefaultSprite") //identity
		{}
	private:
	};
#endif

    void Test();
void Test()
{
	TEST_START
    
	WinDriver winDriver("GLRendererThreadedTest");
	Renderer renderer(winDriver.GetWindowsHandle());
	//MyRenderObject2D obj;
	//RenderObject2DList objList;
	//objList.push_back(&obj);

	const size_t TICKS_PER_SECOND = 30;
	const size_t SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

	size_t next_game_tick = GCL::Time::GetTickMs();
	int loops;

	bool game_is_running = true;
	while( game_is_running )
	{
		loops = 0;
		while(GCL::Time::GetTickMs() > next_game_tick && loops < MAX_FRAMESKIP)
		{
			//perform logic at 60fps
			//obj.Update();

			std::cout <<"update"<<std::endl;
			//flush command buffer to render thread and activate it.
			renderer.PreRender();
			//renderer.Render(RenderObjectList());
			renderer.PostRender();
			ThreadManager::ReThrowException();

			next_game_tick += SKIP_TICKS;
			loops++;
		}
	}
}
}
