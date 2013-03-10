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

#include <applayer/GCLApplication.h>
#include <applayer/GCLRenderObject2D.h>
#include <gcl/Exception.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/GLRenderer.h>
#include <windriver/ViewPort.h>


using namespace GCL;

static const size_t NUM_ROW 	= 5;
static const size_t NUM_COL 	= 8;
static const size_t BORDER_SIZE = 0;
static const size_t BASE_X_RESOLUTION = 1024;
static const size_t BASE_Y_RESOLUTION = 768;
static const Real X_RATIO = (Real)ViewPort::DEFAULT_SCREEN_WIDTH / (Real)BASE_X_RESOLUTION;
static const Real Y_RATIO = (Real)ViewPort::DEFAULT_SCREEN_HEIGHT / (Real)BASE_Y_RESOLUTION;

static const WorldPoint3 START_POSITION(640.0*X_RATIO, 700.0*Y_RATIO, 0.0);
static const WorldPoint3 BALL_INITIAL_VELOCITY(-6.0, -3.0, 0.0);

class AGameObject
{
public:
	AGameObject(const char *objName, const char *objSpriteName)
	: mRenderObject(objName, objSpriteName)
	{
		//mRenderObject.SetScale(WorldPoint2(0.5, 0.5));
	}
	void SetPosition(const WorldPoint3 &position) { mRenderObject.SetPosition(position); }
	const WorldPoint3 &GetPosition() const { return mRenderObject.GetPosition(); }
protected:
	GCLRenderObject2D mRenderObject;
};

class Block : public AGameObject
{
public:
	Block(size_t blockIndex)
	: AGameObject("Block","Block")
	{
		std::stringstream s;
		s.str("Block");
		s << blockIndex;
		mRenderObject.SetName(s.str().c_str());
		const ViewPort &viewPort = GCLApplication::GetWinDriver()->GetViewPort();

		size_t blockWidth = mRenderObject.GetWidth();
		size_t blockHeight = mRenderObject.GetHeight();
		Real blockHalfWidth = blockWidth/2.0;
		Real blockHalfHeight = blockHeight/2.0;

		size_t spaceX = (viewPort.GetWidth())/NUM_COL;
		size_t spaceY = blockHeight;

		Real x = (int(blockIndex%NUM_COL)*spaceX) + BORDER_SIZE + blockHalfWidth;
		Real y = (int(blockIndex/NUM_COL)*spaceY) + BORDER_SIZE + blockHalfHeight;

		//
		WorldPoint3 position(x,y, 0.0);
		SetPosition(position);
		std::cout << position << std::endl;
	}

	void Hit()
	{
		mRenderObject.SetVisible(false);
	}

	bool IsVisible() const { return mRenderObject.IsVisible(); }
	size_t GetHeight() const { return mRenderObject.GetScaledHeight(); }
	size_t GetWidth() const { return mRenderObject.GetScaledWidth(); }
private:
};
typedef std::vector<Block *> BlockList;

class Paddle : public AGameObject
{
public:
	Paddle()
	: AGameObject("Paddle", "Paddle")
	{
		SetPosition(START_POSITION);
	}

	void Update()
	{
		const ViewPort &viewPort = GCLApplication::GetWinDriver()->GetViewPort();

		WorldPoint3 tempPosition = GetPosition();
		if (Input::IsKeyUp(GCL_LEFT))
			tempPosition.x -=5;

		if (Input::IsKeyUp(GCL_RIGHT))
			tempPosition.x+=5.;

		Real paddleHalfWidth = (mRenderObject.GetScaledWidth()/2.0);
		if (tempPosition.x+paddleHalfWidth > viewPort.GetWidth())
			tempPosition.x = viewPort.GetWidth()-paddleHalfWidth;
		else if (tempPosition.x-paddleHalfWidth < 0.0)
			tempPosition.x = 0.+paddleHalfWidth;

		SetPosition(tempPosition);
	}
	size_t GetHeight() const { return mRenderObject.GetScaledHeight(); }
	size_t GetWidth() const { return mRenderObject.GetScaledWidth(); }

};


class Ball : public AGameObject
{
public:

	Ball()
	: AGameObject("Ball", "Ball"),
	  mVelocity(BALL_INITIAL_VELOCITY)
	{
		SetPosition(START_POSITION);
	}
	void Update()
	{
		const ViewPort &viewPort = GCLApplication::GetWinDriver()->GetViewPort();

		WorldPoint3 newPosition = GetPosition()+ mVelocity;

		//walls check
		if (newPosition.x > viewPort.GetWidth())
		{
			mVelocity.x *= -1.0;
			newPosition.x = (Real)viewPort.GetWidth();
		}
		else if (newPosition.x < 0.0)
		{
			mVelocity.x *= -1.0;
			newPosition.x = 0.0;
		}
		if (newPosition.y < 0.0)
		{
			mVelocity.y *= -1.0;
			newPosition.y = 0.0;
		}
		//check if we're dead and reset
		else if (newPosition.y > viewPort.GetHeight())
		{
			mVelocity *= -1.0;
			newPosition = START_POSITION;
		}
		SetPosition(newPosition);
	}
	WorldPoint3 mVelocity;
};


static void PerformBallPaddleCollision(Ball &ball, Paddle &paddle)
{
	const WorldPoint3 &ballPosition = ball.GetPosition();
	const WorldPoint3 &paddlePosition = paddle.GetPosition();
	size_t paddleHeight = paddle.GetHeight();
	size_t paddleWidth = paddle.GetWidth();
	Real paddleHalfWidth = paddleWidth / 2.0;
	Real paddleHalfHeight = paddleHeight / 2.0;
	if (ballPosition.y > paddlePosition.y-paddleHalfHeight)
	{
		if (ballPosition.x > paddlePosition.x-paddleHalfWidth &&
				ballPosition.x < paddlePosition.x + paddleHalfWidth)
		{
			if (ballPosition.y < paddlePosition.y+paddleHalfHeight)
			{
				WorldPoint3 newBallPosition = ballPosition;
				ball.mVelocity.y *= -1.0;
				newBallPosition.y = paddlePosition.y-paddleHalfHeight;
				ball.SetPosition(newBallPosition);
			}
		}
	}
}

static void PerformBallBlocksCollision(Ball &ball, BlockList &blockList)
{

	const WorldPoint3 &ballPosition = ball.GetPosition();
	for (size_t i=0; i<blockList.size(); ++i)
	{
		Block &currentBlock = *(blockList[i]);
		if (!currentBlock.IsVisible())
			continue;
		const WorldPoint3 &blockPosition = currentBlock.GetPosition();
		size_t blockHeight = currentBlock.GetHeight();
		size_t blockWidth = currentBlock.GetWidth();
		Real blockHalfWidth = blockWidth/ 2.0;
		Real blockHalfHeight = blockHeight/ 2.0;

		if (ballPosition.y > blockPosition.y-blockHalfHeight &&
				ballPosition.y < blockPosition.y+blockHalfHeight &&
				ballPosition.x > blockPosition.x-blockHalfWidth &&
				ballPosition.x < blockPosition.x+blockHalfWidth )
		{
			//we have collision need to figure out what edge we hit
			ball.mVelocity *= -1.0;
			currentBlock.Hit();
		}

	}
}
int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize("Arkanoid");
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		BlockList blocks;
		for (size_t i=0; i<NUM_ROW*NUM_COL; ++i)
		{
			blocks.push_back(new Block(i));
		}
		Paddle paddle;
		Ball ball;


		bool isRunning = true;
		while (isRunning)
		{
			PerformBallPaddleCollision(ball, paddle);
			PerformBallBlocksCollision(ball, blocks);

			ball.Update();
			paddle.Update();
			GCLApplication::Update();

			if (Input::IsKeyUp(GCL_ESCAPE))
				isRunning=false;

			GCLApplication::Render();
			Time::SleepMs(100);
			std::cout.flush();
		}
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}


	GCLApplication::Terminate();

	return 0;
}

