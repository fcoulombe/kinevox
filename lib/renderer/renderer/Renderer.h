#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>

namespace GCL
{
  class RenderObject;
  class Sprite;
  typedef std::vector<const RenderObject*> RenderObjectList;
  typedef std::vector<Sprite*> SpriteList;
  class Renderer
  {
  public:
    virtual ~Renderer() {}
    virtual bool Update() =0;
    virtual void Render(const RenderObjectList &renderObjectList) =0;
    virtual void Render(const SpriteList &renderObjectList) =0;
    virtual void Render(uint8_t *rgb_front, uint8_t *depth_front) =0;
  protected:
  };
}
