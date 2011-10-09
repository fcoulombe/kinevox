#pragma once

namespace GCL
{
  struct VertexData
  {
    const void *mVertexData;
    size_t vertexCount;
    int vertexType;

  };

  class RenderObject
  {
  public:
    RenderObject()
    {
    }
    virtual const VertexData *GetVertexData() const=0;
  };
}
