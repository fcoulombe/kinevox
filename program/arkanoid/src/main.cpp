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

#include <unistd.h>
#include <cstdlib>
#include <renderer/OpenCV.h>
#include <SDL.h>

#include <applayer/GCLApplication.h>
#include <applayer/GCLRenderObject2D.h>
#include <gcl/Exception.h>
#include <gcl/Point3.h>
#include <input/Input.h>
#include <renderer/Camera.h>
#include <renderer/Material.h>
#include <renderer/GLRenderer.h>


using namespace GCL;

static const size_t NUM_ROW = 10;
static const size_t NUM_COL = 20;
static const size_t BORDER_SIZE = 5;

class AGameObject
{
public:
	AGameObject(const char *objName, const char *objSpriteName)
	: mRenderObject(objName, objSpriteName)
	{
		mRenderObject.SetScale(WorldPoint2(0.5, 0.5));
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
		const ViewPort &viewPort = GCLApplication::GetRenderer()->GetViewPort();

		size_t spaceX = viewPort.GetWidth()/NUM_COL;
		size_t spaceY = viewPort.GetHeight()/NUM_ROW;
		(void)spaceX;
		(void)spaceY;
		size_t y = blockIndex/NUM_ROW;
		size_t x = blockIndex%NUM_ROW;

		WorldPoint3 position(x, y, 0.0);
		SetPosition(position);
		std::cout << position << std::endl;
	}
private:
};

class Paddle : public AGameObject
{
public:
	Paddle()
	: AGameObject("Paddle", "Paddle")
	{
		SetPosition(WorldPoint3(640.0, 500.0, 0.0));
	}

	void Update()
	{
		const ViewPort &viewPort = GCLApplication::GetRenderer()->GetViewPort();

		WorldPoint3 tempPosition = GetPosition();
		if (Input::IsKeyUp(SDLK_LEFT))
			tempPosition.x -=5;

		if (Input::IsKeyUp(SDLK_RIGHT))
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
	  mVelocity(-3.0, -1.5, 0.0)
	{
		SetPosition(WorldPoint3(640.0, 500.0, 0.0));
	}
	void Update()
	{
		const ViewPort &viewPort = GCLApplication::GetRenderer()->GetViewPort();

		WorldPoint3 newPosition = GetPosition()+ mVelocity;
		if (newPosition.x > viewPort.GetWidth())
		{
			mVelocity.x *= -1.0;
			newPosition.x = viewPort.GetWidth();
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
		SetPosition(newPosition);
	}
	WorldPoint3 mVelocity;
};

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize();
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		typedef std::vector<Block *> BlockList;
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


			ball.Update();
			paddle.Update();
			GCLApplication::Update();

			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;

			GCLApplication::Render();
			usleep(100);
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

