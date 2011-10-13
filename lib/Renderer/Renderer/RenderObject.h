#pragma once

#include <GCL/Matrix44.h>

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
    RenderObject(const Matrix44 &transform)
    : mTransform(transform)
    {
    }
    virtual ~RenderObject() {}
    virtual const VertexData &GetVertexData() const=0;
    const Matrix44 &GetTransform() const {return mTransform; }

  protected:
    Matrix44 mTransform;
  };
}
