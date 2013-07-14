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
#include <gcl/Assert.h>
#include <gcl/Point3.h>

#define MeshReal float
namespace GCL
{
  enum VertexComponents
  {
    ePOSITION 	        = 1<<0,
    eNORMAL 	        = 1<<1,
    eTEXTURE_COORD 	= 1<<2
  };

  struct VertexP
  {
    Point3<MeshReal> position;
    static size_t GetComponentType()  { return ePOSITION; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToTextureCoordinate() { GCLAssert(false); return 0; }
    static size_t OffsetToNormal() { GCLAssert(false);return 0; }
  };

  struct VertexPT
  {
    Point3<MeshReal> position;
    Point2<MeshReal> textureCoordinate;
    static size_t GetComponentType() { return ePOSITION|eTEXTURE_COORD; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToTextureCoordinate() { return sizeof(Point3<MeshReal>); }
    static size_t OffsetToNormal() { GCLAssert(false);return 0; }

  };

  struct VertexPNT
  {
    Point3<MeshReal> position;
    Point3<MeshReal> normal;
    Point2<MeshReal> textureCoordinate;
    static size_t GetComponentType()  { return ePOSITION|eNORMAL|eTEXTURE_COORD; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToNormal() { return sizeof(Point3<MeshReal>); }
    static size_t OffsetToTextureCoordinate() { return sizeof(Point3<MeshReal>)+sizeof(Point3<MeshReal>); }
  };

  struct VertexPN
  {
    Point3<MeshReal> position;
    Point3<MeshReal> normal;
    static size_t GetComponentType()  { return ePOSITION|eNORMAL; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToNormal() { return sizeof(Point3<MeshReal>); }
    static size_t OffsetToTextureCoordinate() { GCLAssert(false); return 0; }
  };

}
