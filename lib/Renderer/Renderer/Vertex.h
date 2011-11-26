#pragma once
#include <GCL/Assert.h>
#include <GCL/Point3.h>

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
    WorldPoint3 position;
    static uint32_t GetComponentType()  { return ePOSITION; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToTextureCoordinate() { GCLAssert(false); return 0; }
    static size_t OffsetToNormal() { GCLAssert(false);return 0; }
  };

  struct VertexPT
  {
    WorldPoint3 position;
    WorldPoint2 textureCoordinate;
    static uint32_t GetComponentType() { return ePOSITION|eTEXTURE_COORD; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToTextureCoordinate() { return sizeof(WorldPoint3); }
    static size_t OffsetToNormal() { GCLAssert(false);return 0; }

  };

  struct VertexPNT
  {
    WorldPoint3 position;
    WorldPoint3 normal;
    WorldPoint2 textureCoordinate;
    static uint32_t GetComponentType()  { return ePOSITION|eNORMAL|eTEXTURE_COORD; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToNormal() { return sizeof(WorldPoint3); }
    static size_t OffsetToTextureCoordinate() { return sizeof(WorldPoint3)+sizeof(WorldPoint2); }
  };

  struct VertexPN
  {
    WorldPoint3 position;
    WorldPoint3 normal;
    static uint32_t GetComponentType()  { return ePOSITION|eNORMAL; }
    static size_t OffsetToPosition() { return 0; }
    static size_t OffsetToNormal() { return sizeof(WorldPoint3); }
    static size_t OffsetToTextureCoordinate() { GCLAssert(false); return 0; }
  };

}
