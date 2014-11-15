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
#include <applayer/Sprite.h>
#include <gcl/Exception.h>
#include <gcl/Log.h>
#include <gcl/Time.h>
#include <input/Input.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize("Sprite Example");
        Sprite car("Car");
        static const WorldPoint3 kCarPositionTest(120.0, 240.0, 0.0);
        car.SetPosition(kCarPositionTest);

		Sprite obj;
		obj.Play();

		static const WorldPoint3 kPositionTest(320.0, 240.0, 0.0);
		obj.SetPosition(kPositionTest);

		bool isRunning = true;
		const size_t TICKS_PER_SECOND = 60;
		const size_t SKIP_TICKS = 1000 / TICKS_PER_SECOND;
		const int MAX_FRAMESKIP = 5;
		size_t next_game_tick = GCL::Time::GetTickMs()-1;
		int loops;
		while(isRunning)
		{
			loops = 0;
			while(GCL::Time::GetTickMs() > next_game_tick && loops < MAX_FRAMESKIP)
			{

				GCLApplication::Update();
				obj.Update();
                car.Update();
				if (Input::IsKeyUp(GCL_ESCAPE))
					isRunning=false;

				if (Input::IsKeyUp(GCL_LEFT))
				{
					WorldPoint3 position = obj.GetPosition();
					position.x -= 1.0;
					obj.SetPosition(position);
				}
				if (Input::IsKeyUp(GCL_RIGHT))
				{
					WorldPoint3 position = obj.GetPosition();
					position.x += 1.0;
					obj.SetPosition(position);
				}
				if (Input::IsKeyUp(GCL_UP))
				{
					WorldPoint3 position = obj.GetPosition();
					position.y -= 1.0;
					obj.SetPosition(position);
				}
				if (Input::IsKeyUp(GCL_DOWN))
				{
					WorldPoint3 position = obj.GetPosition();
					position.y += 1.0;
					obj.SetPosition(position);
				}
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

