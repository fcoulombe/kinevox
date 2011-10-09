#pragma once
#include <stdint.h>
#include <vector>

namespace GCL
{
  class RenderObject;
  class Renderer
  {
  public:
    virtual ~Renderer() {}
    virtual bool Update() =0;
    virtual void Render(uint8_t *rgb_front, uint8_t *depth_front) =0;

    void RegisterRenderObject(const RenderObject *renderObject)
    {
      mRenderObjectList.push_back(renderObject);
    }

  protected:
    typedef std::vector<const RenderObject*> RenderObjectList;
    RenderObjectList mRenderObjectList;
  };
}
