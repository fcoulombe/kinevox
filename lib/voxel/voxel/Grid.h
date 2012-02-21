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

#pragma once

#include <vector>

#include <gcl/Point3.h>
#include <renderer/RenderObject.h>
#include <renderer/Material.h>

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
    const VertexData &GetVertexData() const; // this function can't be const because it might need to create vertex data on demand

    const Material &GetMaterial() const { return mMaterial; }
  private:

    Material mMaterial;
    const size_t X_DIMENSION;
    const size_t Y_DIMENSION;
    const size_t Z_DIMENSION;

    VertexData mVertexData;
    typedef std::vector<Slice> SliceList;
    SliceList mSlices;
  };
}
