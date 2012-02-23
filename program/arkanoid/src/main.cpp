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
	AGameObject(const char *objSpriteName)
	: mRenderObject(objSpriteName)
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
	: AGameObject("Block")
	{
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
	: AGameObject("Paddle")
	{
		SetPosition(WorldPoint3(640.0, 500.0, 0.0));
	}
};


class Ball : public AGameObject
{
public:
	Ball()
	: AGameObject("Ball")
	{
		SetPosition(WorldPoint3(640.0, 500.0, 0.0));
	}
	void Update()
	{
		SetPosition(GetPosition() + mVelocity);
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
		for (size_t i=0; i<blocks.size(); ++i)
		{
			blocks.push_back(new Block(i));
		}
		Paddle paddle;
		Ball ball;


		bool isRunning = true;
		while (isRunning)
		{
			ball.Update();

			GCLApplication::Update();
			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;

			WorldPoint3 tempPosition;
			tempPosition= paddle.GetPosition();
			if (Input::IsKeyUp(SDLK_LEFT))
				tempPosition.x -=5;

			if (Input::IsKeyUp(SDLK_RIGHT))
				tempPosition.x+=5.;

			paddle.SetPosition(tempPosition);

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

