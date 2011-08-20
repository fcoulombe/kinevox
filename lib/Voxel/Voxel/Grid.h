#pragma once

#include <GCL/Point3.h>
#include <Renderer/RenderObject.h>
#include <Renderer/GLRenderUtils.h>

#include "Voxel/Voxel.h"

namespace GCL
{



template<int X_DIMENSION, int Y_DIMENSION, int Z_DIMENSION>
class Grid : public RenderObject
{
public:
	Grid()
	{
		for (size_t i=0; i<X_DIMENSION; ++i) {
			for (size_t j=0; j<Y_DIMENSION; ++j) {
				for (size_t k=0; k<Z_DIMENSION; ++k) {
					mGridVoxelPosition[i][j][k] = WorldPoint3(i*gVoxelSize,j*gVoxelSize,k*gVoxelSize);
				}
			}
		}
	}

	const void *GetVertexData() const
	{
		for (size_t i=0; i<X_DIMENSION; ++i) {
			for (size_t j=0; j<Y_DIMENSION; ++j) {
				for (size_t k=0; k<Z_DIMENSION; ++k) {
					DrawCube(mGridVoxelPosition[i][j][k], gVoxelSize);
				}
			}
		}

		return NULL;
	}



private:

	static const double gVoxelSize = 1.0;
	Voxel mGrid[X_DIMENSION][Y_DIMENSION][Z_DIMENSION];
	WorldPoint3 mGridVoxelPosition[X_DIMENSION][Y_DIMENSION][Z_DIMENSION];

};
}
