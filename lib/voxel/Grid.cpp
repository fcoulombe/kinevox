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

#include "voxel/Grid.h"

#include <gcl/Assert.h>
#include <renderer/GLRenderUtils.h>
#include <renderer/Vertex.h>
#include "voxel/Voxel.h"

using namespace GCL;


namespace
{
const Real gVoxelSize = 1.0;
}

Grid::Row::Row(size_t x, size_t y, size_t z)
{
	mRow.resize(x);
	mPositions.reserve(x);
	for (size_t i = 0; i<x; ++i)
	{
		mPositions.push_back(WorldPoint3(i*gVoxelSize,y*gVoxelSize,z*gVoxelSize));
	}
}

Grid::Row::~Row()
{
}



Grid::Slice::Slice(size_t x, size_t y, size_t z)
{
	mRows.reserve(y);
	for (size_t i=0; i<y; ++i)
	{
		mRows.push_back(Row(x,y,z));
	}
}

Grid::Slice::~Slice()
{
}


Grid::Grid(size_t x, size_t y, size_t z)
: RenderObject("Grid", Matrix44(true)), //identity
  X_DIMENSION(x),
  Y_DIMENSION(y),
  Z_DIMENSION(z)
{
	mSlices.reserve(z);
	for (size_t i=0; i<z; ++i)
	{
		mSlices.push_back(Slice(x,y,z));
	}
}

Grid::~Grid()
{
	if (mVertexData.mVertexData)
		delete [] (WorldPoint3 *)(mVertexData.mVertexData);

}

void Grid::CreateVertexData()
{
	if (mVertexData.mVertexData)
		delete [] (WorldPoint3 *)(mVertexData.mVertexData);
	WorldPoint3 *vertexData = new WorldPoint3[4*6*X_DIMENSION*Y_DIMENSION*Z_DIMENSION];
	WorldPoint3 *currentCube=vertexData;
	for (size_t i=0; i<Z_DIMENSION; ++i)
	{
		const Slice &tempSlice = mSlices[i];
		for (size_t j=0; j<Y_DIMENSION; ++j)
		{
			const Row &tempRow = tempSlice.mRows[j];
			for (size_t k=0; k<X_DIMENSION; ++k)
			{
				WriteCube(currentCube, tempRow.mPositions[k], gVoxelSize);
				currentCube += 4*6;
				//DrawCube(mGridVoxelPosition[i][j][k], gVoxelSize);
			}
		}
	}
	mVertexData.mVertexData = (const void *)vertexData;
	mVertexData.mVertexCount = 4*6*X_DIMENSION*Y_DIMENSION*Z_DIMENSION;
	mVertexData.mVertexType = VertexP::GetComponentType();
}

const VertexData &Grid::GetVertexData() const
{
	GCLAssert(mVertexData.mVertexData);
	return mVertexData;
}

