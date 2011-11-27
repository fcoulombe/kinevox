#pragma once

#include <stdint.h>
#include <gcl/Matrix44.h>


namespace GCL
{
class Material;
  struct VertexData
  {
    const void *mVertexData;
    size_t vertexCount;
    uint32_t vertexType;

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
    virtual const Material &GetMaterial() const=0;
    const Matrix44 &GetTransform() const {return mTransform; }

  protected:
    Matrix44 mTransform;
  };
}
