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

#include <applayer/GameState.h>


namespace Arkanoid
{
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
:GameState("MainGame")
{
		std::vector<Actor*> blocks;
		for (size_t i=0; i<NUM_ROW*NUM_COL; ++i)
		{
			Actor *block = new Actor("Block", "Block");
			blocks.push_back(block);
			Sprite & sprite = *static_cast<SpriteComponent*>(block->GetComponent("SpriteComponent"))->GetSprite();

			size_t blockWidth = sprite.GetWidth();
			size_t blockHeight = sprite.GetHeight();
			Real blockHalfWidth = blockWidth/2.0;
			Real blockHalfHeight = blockHeight/2.0;

			size_t spaceX = (VIEWPORT_WIDTH)/NUM_COL;
			size_t spaceY = blockHeight;

			Real x = (int(i%NUM_COL)*spaceX) + BORDER_SIZE + blockHalfWidth;
			Real y = (int(i/NUM_COL)*spaceY) + BORDER_SIZE + blockHalfHeight;

			//
			WorldPoint3 position(x,y, 0.0);
			block->SetPosition(position);
			std::cout << position << std::endl;
		}
		Actor paddle("Paddle", "Paddle");
		paddle.SetPosition(START_POSITION);
		Actor ball("Ball", "Ball");
		ball.SetPosition(START_POSITION);
}
	bool Update(Real dt)
	{
		hasBeenInMainGameState = true;

		if (mFramecount>3)
			PushChildState(std::make_shared<PauseState>());
		++mFramecount;
		GameState::Update(dt);
		return true;
	}


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
}
