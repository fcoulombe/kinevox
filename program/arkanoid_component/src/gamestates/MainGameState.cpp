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


#include "MainGameState.h"


#include <applayer/Actor.h>
#include <applayer/GameStateManager.h>
#include <applayer/SpriteComponent.h>
#include <applayer/Sprite.h>
#include <gcl/Config.h>
#include <gcl/Exception.h>
#include <gcl/Time.h>
#include <input/Input.h>

using namespace GCL;
using namespace Arkanoid;

static const size_t NUM_ROW 	= 5;
static const size_t NUM_COL 	= 8;
static const size_t BORDER_SIZE = 0;
static const size_t BASE_X_RESOLUTION = 1024;
static const size_t BASE_Y_RESOLUTION = 768;
static const size_t VIEWPORT_WIDTH =  Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH") ;
static const size_t VIEWPORT_HEIGHT =  Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT") ;
static const Real X_RATIO = (Real)VIEWPORT_WIDTH/ (Real)BASE_X_RESOLUTION;
static const Real Y_RATIO = (Real)VIEWPORT_HEIGHT / (Real)BASE_Y_RESOLUTION;

static const WorldPoint3 START_POSITION(640.0*X_RATIO, 700.0*Y_RATIO, 0.0);
static const WorldPoint3 BALL_INITIAL_VELOCITY(-6.0, -3.0, 0.0);

MainGameState::~MainGameState()
{
	for (size_t i=0; i<blocks.size(); ++i)
	{
		delete blocks[i];
	}
	blocks.clear();

	delete paddle;
	delete ball;
}

MainGameState::MainGameState()
:GameState("MainGame")
{
	std::cout << "EnterMainGameState"<<std::endl;
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
	paddle = new Actor("Paddle", "Paddle");
	ball = new Actor("Ball", "Ball");
	paddle->SetPosition(START_POSITION);

	ball->SetPosition(START_POSITION+WorldPoint3(100,0,0));
}
bool MainGameState::Update(Real dt)
{
	//std::cout << "MainGameState";
	for (size_t i=0; i<NUM_ROW*NUM_COL; ++i)
	{
		blocks[i]->Update(dt);
	}
	paddle->Update(dt);
	ball->Update(dt);
	GameState::Update(dt);

	return true;
}

