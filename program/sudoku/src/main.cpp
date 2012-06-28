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
#include <sstream>

#include <applayer/GCLApplication.h>
#include <applayer/GCLRenderObject2D.h>
#include <gcl/Exception.h>
#include <input/Input.h>
#include <renderer/GLRenderer.h>
#include <renderer/ViewPort.h>


using namespace GCL;

static const size_t NUM_ROW 	=9;
static const size_t NUM_COL 	=9;
static const size_t BORDER_SIZE = 0;
static const size_t BASE_X_RESOLUTION = 1024;
static const size_t BASE_Y_RESOLUTION = 768;
static const Real X_RATIO = (Real)ViewPort::DEFAULT_SCREEN_WIDTH / (Real)BASE_X_RESOLUTION;
static const Real Y_RATIO = (Real)ViewPort::DEFAULT_SCREEN_HEIGHT / (Real)BASE_Y_RESOLUTION;

static const WorldPoint3 START_POSITION(640.0*X_RATIO, 700.0*Y_RATIO, 0.0);
static const WorldPoint3 BALL_INITIAL_VELOCITY(-6.0, -3.0, 0.0);

class SudokuGameObject
{
public:
	SudokuGameObject(const char *objName, const char *objSpriteName)
	: mRenderObject(objName, objSpriteName)
	{
		//mRenderObject.SetScale(WorldPoint2(0.5, 0.5));
	}
	void SetPosition(const WorldPoint3 &position) { mRenderObject.SetPosition(position); }
	const WorldPoint3 &GetPosition() const { return mRenderObject.GetPosition(); }
protected:
	GCLRenderObject2D mRenderObject;
};

class Group;
class Column;
class Row;
class Cell : public SudokuGameObject
{
public:
	Cell(size_t cellIndex)
	: SudokuGameObject("Cell","Cell"),
	  mCellIndex(cellIndex)
	{
		std::stringstream s;
		s.str("Cell");
		s << mCellIndex;
		mRenderObject.SetName(s.str().c_str());
		const ViewPort &viewPort = GCLApplication::GetRenderer()->GetViewPort();

		size_t blockWidth = mRenderObject.GetWidth();
		size_t blockHeight = mRenderObject.GetHeight();
		Real blockHalfWidth = blockWidth/2.0;
		Real blockHalfHeight = blockHeight/2.0;

		size_t spaceX = (viewPort.GetWidth())/NUM_COL;
		size_t spaceY = blockHeight;

		Real x = (int(mCellIndex%NUM_COL)*spaceX) + BORDER_SIZE + blockHalfWidth;
		Real y = (int(mCellIndex/NUM_COL)*spaceY) + BORDER_SIZE + blockHalfHeight;

		//
		WorldPoint3 position(x,y, 0.0);
		SetPosition(position);
		std::cout << position << std::endl;
	}

	size_t GetHeight() const { return mRenderObject.GetScaledHeight(); }
	size_t GetWidth() const { return mRenderObject.GetScaledWidth(); }
	size_t GetCellIndex() const { return mCellIndex; }
private:
	size_t mCellIndex;
	Group *mGroup;
	Column *mColumn;
	Row *mRow;
};
typedef std::vector<Cell *> CellList;
class Group
{
public:
	CellList mCellList;
};
class Column
{
public:
	CellList mCellList;
};
class Row
{
public:
	CellList mCellList;
};

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		GCLApplication::Initialize();
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		CellList cells;
		for (size_t i=0; i<NUM_ROW*NUM_COL; ++i)
		{
			cells.push_back(new Cell(i));
		}

		const size_t halfCellWidth = cells[0]->GetWidth()/2;
		const size_t halfCellHeight = cells[0]->GetHeight()/2;

		bool isRunning = true;
		while (isRunning)
		{
			GCLApplication::Update();

			if (Input::IsKeyUp(SDLK_ESCAPE))
				isRunning=false;
			size_t x = Input::GetMouseX() ;
			size_t y = Input::GetMouseY() ;
			for (size_t i=0; i<cells.size(); ++i)
			{
				Cell *tempCell = cells[i];
				const WorldPoint3 &pos = tempCell->GetPosition();
				if (x<pos.x+halfCellWidth &&
						x>pos.x-halfCellWidth &&
						y>pos.y-halfCellHeight &&
						y<pos.y+halfCellHeight)
				{
					std::cout<<"X: "<<x<<" Y: "<<y<<std::endl;
					std::cout << "Cell: " << tempCell->GetCellIndex() << std::endl;
				}
			}


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

