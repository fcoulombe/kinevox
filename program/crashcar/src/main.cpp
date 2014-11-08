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

#include <cstdlib>
#include <sstream>
#ifdef USE_VLD
#include <vld.h>
#endif
#include <applayer/GCLApplication.h>
#include <applayer/GameStateManager.h>
#include <applayer/ScriptedGameState.h>
#include <gcl/Time.h>
#include <input/Input.h>


using namespace GCL;

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize("Crash Car");
		GameStateManager::ChangeToState(std::make_shared<ScriptedGameState>("SplashState", "SplashState"));
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

				if (Input::IsKeyUp(GCL_ESCAPE))
					isRunning=false;

				GCLApplication::Render();
				std::cout.flush();
				next_game_tick += SKIP_TICKS;
				loops++;
			}
		}
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}


	GCLApplication::Terminate();

	return 0;
}

