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

#include <applayer/GCLApplication.h>
#include <applayer/Actor.h>
#include <gcl/Exception.h>
#include <gcl/Log.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/Camera.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize("Actor Example");
		Actor obj("Mario", "Mario");
		obj.SetPosition(0.0,0.0,-10.0);
		Camera cam;

		bool isRunning = true;
		const size_t TICKS_PER_SECOND = 60;
		const size_t SKIP_TICKS = 1000 / TICKS_PER_SECOND;
		const int MAX_FRAMESKIP = 5;
		size_t next_game_tick = GCL::Time::GetTickMs()-1;
		int loops;
		Real x=0., y=0.,z=0.;
		while(isRunning)
		{
			loops = 0;
			while(GCL::Time::GetTickMs() > next_game_tick && loops < MAX_FRAMESKIP)
			{
				GCLApplication::SetViewportCamera(cam);
				GCLApplication::Update();
				if (Input::IsKeyUp(GCL_ESCAPE))
					isRunning=false;
				const Real kSpeed = 0.1;
				if (Input::IsKeyUp(GCL_LEFT))
				{
					y+=kSpeed;
				}
				if (Input::IsKeyUp(GCL_RIGHT))
				{
					y-=kSpeed;
				}
				if (Input::IsKeyUp(GCL_UP))
				{
					x+=kSpeed;
				}
				if (Input::IsKeyUp(GCL_DOWN))
				{
					x-=kSpeed;
				}
				obj.SetOrientation(x,y,z);
				GCLApplication::Render();
				std::cout.flush();
				next_game_tick += SKIP_TICKS;
				loops++;
			}
		}
	}
	catch(GCLException &e)
	{
		KLog("%s", e.what());
	}


	GCLApplication::Terminate();

	return 0;
}

