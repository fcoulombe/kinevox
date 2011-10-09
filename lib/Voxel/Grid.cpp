#include "Voxel/Grid.h"

#include <GCL/Assert.h>
#include <Renderer/GLRenderUtils.h>
#include <Renderer/Vertex.h>
#include "Voxel/Voxel.h"

using namespace GCL;


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
: X_DIMENSION(x),
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
              WriteCube(currentCube, tempRow.mPositions[k], Row::gVoxelSize);
              currentCube += 4*6;
              //DrawCube(mGridVoxelPosition[i][j][k], gVoxelSize);
            }
        }
    }
  mVertexData.mVertexData = (const void *)vertexData;
  mVertexData.vertexCount = 4*6*X_DIMENSION*Y_DIMENSION*Z_DIMENSION;
  mVertexData.vertexType = VertexP::GetComponentType();
}

const VertexData *Grid::GetVertexData() const
{
  GCLAssert(mVertexData.mVertexData);
  return &mVertexData;
}

