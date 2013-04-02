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
#include <stdint.h>
#include <cstdlib>
#include <vector>

#include <gcl/Config.h>
#include "renderer/ViewPort.h"

namespace GCL
{
  class RenderObject;
  class RenderObject2D;
  class Text2D;
  typedef std::vector<const RenderObject*> RenderObjectList;
  typedef std::vector<RenderObject2D*> RenderObject2DList;
  typedef std::vector<Text2D*> Text2DList;
  class Renderer
  {
  public:
      Renderer()
      {
          mViewPort.Set(0,0,Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"));
      }
    virtual ~Renderer() {}
    virtual bool Update() =0;
    virtual void Render(const RenderObjectList &renderObjectList) =0;
    virtual void Render(const RenderObject2DList &renderObjectList) =0;
    virtual void Render(const Text2DList &text2DList) =0;
    virtual void Render(uint8_t *rgb_front, uint8_t *depth_front) =0;
    const ViewPort &GetViewPort() const { return mViewPort; }
  protected:
      ViewPort mViewPort;
  };
}
