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

#include <GCL/UnitTest.h>
#include <Renderer/Texture.h>
#include <Renderer/Shader.h>
#include <Renderer/Vertex.h>

using namespace GCL;
namespace TextureAndShaderTest
{
  TEST_START

  class MyRenderObject : public RenderObject
  {
  public:
    MyRenderObject()
    : RenderObject(Matrix44::IDENTITY) {}
    const VertexData &GetVertexData() const
    {
      static const   VertexPNT square[4] = {
          {WorldPoint3(-0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)} ,
          {WorldPoint3(0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
          {WorldPoint3(0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
          {WorldPoint3(-0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } };
      static const VertexData data = {&square, 4, VertexPNT::GetComponentType()};
      return data;

    }
  private:


  };

  bool CompareImages(const char * /*filename1*/, const char * /*filename2*/)
  {
    Assert_Test(false && "TBD");
    return false;
  }
  void Test()
  {
    TextureResourceManager::Initialize();
    OpenGLRenderer renderer;
    Shader shader;
    shader.Bind();

    Texture texture("data/mushroom.png");
    texture.Bind();

    MyRenderObject obj;

    renderer.Render();


    /*        target.Save("RenderTargetTest.tga");
    Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));
*/
    TextureResourceManager::Terminate();

  }
}
