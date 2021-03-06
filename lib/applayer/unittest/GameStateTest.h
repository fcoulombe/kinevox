/*
 * Copyright (C) 2014 by Francois Coulombe
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

#include <gcl/UnitTest.h>
#include <applayer/Actor.h>
#include <applayer/GameState.h>
#include <applayer/GameStateManager.h>
#include <applayer/GCLApplication.h>



using namespace GCL;
namespace GameStateTest
{

bool hasBeenInMainMenuState=false;
bool hasBeenInConfigState=false;
bool  hasBeenInMainGameState=false;
bool  hasBeenInPauseState=false;
bool  hasBeenInSplashState=false;
class PauseState: public GameState
{
public:
	PauseState()
: GameState("Pause")
{}
	bool Update(Real dt)
	{
		hasBeenInPauseState = true;
		GameState::Update(dt);
		return false;
	}
};
class ConfigState: public GameState
{
public:
	ConfigState()
: GameState("config")
{}
	bool Update(Real dt)
	{
		hasBeenInConfigState = true;
		GameState::Update(dt);
		GameStateManager::PopState();
		return true;
	}
};
class MainGameState: public GameState
{
public:
	MainGameState()
:GameState("MainGame"),
 actor("TestActor", "TestActor")
	, mFramecount(0)
{
		actor.SetPosition(0.0,0.0,-10.0);
}
	bool Update(Real dt)
	{
		hasBeenInMainGameState = true;
		GCLApplication::SetViewportCamera(cam); 
		actor.Update(dt);
		if (mFramecount>3)
			PushChildState(std::make_shared<PauseState>());
		++mFramecount;
		GameState::Update(dt);
		return true;
	}

	Actor actor;
	size_t mFramecount;
	Camera cam;
};

class MainMenuState: public GameState
{
public:
	MainMenuState()
: GameState("MainMenu"),
  mFrameCount(0)
{}
	bool Update(Real dt)
	{
		hasBeenInMainMenuState = true;
		if(mFrameCount == 0)
			GameStateManager::PushState(std::make_shared<ConfigState>());
		else if (mFrameCount == 1)
			GameStateManager::ChangeToState(std::make_shared<MainGameState>());
		++mFrameCount;
		GameState::Update(dt);
		return true;
	}
private:
	size_t mFrameCount;
};
class SplashState: public GameState
{
public:
	SplashState()
: GameState("Splash")
{}

	bool Update(Real dt)
	{
		hasBeenInSplashState = true;
		GameStateManager::ChangeToState(std::make_shared<MainMenuState>());
		GameState::Update(dt);
		return true;
	}
};


void Test()
{
	KINEVOX_TEST_START
	GCLApplication::Initialize("GameStateTest");
	{
		GameStateManager::ChangeToState(std::make_shared<SplashState>());
		Assert_Test(GameStateManager::GetCurrentStateName() == "Splash");


		//KINEVOX_TEST_LOOP_START
		for (size_t i=0; i<20; ++i)
		{
			GCLApplication::Update();
			GCLApplication::Render();
		}
		Assert_Test(hasBeenInMainMenuState);
		Assert_Test(hasBeenInConfigState);
		Assert_Test(hasBeenInMainGameState);
		Assert_Test(hasBeenInPauseState);
		Assert_Test(hasBeenInSplashState);
		//KINEVOX_TEST_LOOP_END
	}
	GCLApplication::Terminate();
}
}
