#pragma once

#include <vector>

#include <GCL/Point3.h>
#include <Renderer/RenderObject.h>

namespace GCL
{

  class Voxel;


  class Grid : public RenderObject
  {
  public:
    struct Row
    {
      Row(size_t x, size_t y, size_t z);
      ~Row();

      static const WorldUnit gVoxelSize = 1.0;
      typedef std::vector<Voxel> VoxelList;
      VoxelList mRow;
      typedef std::vector<WorldPoint3> PositionList;
      PositionList mPositions;
    };

    struct Slice
    {
      Slice(size_t x, size_t y, size_t z);
      ~Slice();
      typedef std::vector<Row> RowList;
      RowList mRows;
    };
  public:
    Grid(size_t x, size_t y, size_t z);
    ~Grid();

    void CreateVertexData();
    const VertexData *GetVertexData() const; // this function can't be const because it might need to create vertex data on demand

  private:

    const size_t X_DIMENSION;
    const size_t Y_DIMENSION;
    const size_t Z_DIMENSION;

    VertexData mVertexData;
    typedef std::vector<Slice> SliceList;
    SliceList mSlices;
  };
}
