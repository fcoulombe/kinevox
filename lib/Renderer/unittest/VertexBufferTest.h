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
#include <Renderer/VertexBuffer.h>

using namespace GCL;
namespace VertexBufferTest
{
TEST_START

void Test()
{
  OpenGLRenderer renderer;

  Shader shader;
  shader.Bind();

  VertexPNT square[4] = { {WorldPoint3(-0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
                          {WorldPoint3(0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
                          {WorldPoint3(0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
                          {WorldPoint3(-0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } };
  VertexBuffer<VertexPNT> vb(square, 4);
  Assert_Test(vb.IsValid());


  VertexPT square2[4] = { {WorldPoint3(-0.5, -0.5, 0.0), WorldPoint2(0.0, 0.0)} ,
                          {WorldPoint3(0.5, -0.5, 0.0), WorldPoint2(0.0, 0.0) } ,
                          {WorldPoint3(0.5, 0.5, 0.0), WorldPoint2(0.0, 0.0)} ,
                          {WorldPoint3(-0.5, 0.5, 0.0), WorldPoint2(0.0, 0.0)} };
  VertexBuffer<VertexPT> vb2(square2, 4);
  Assert_Test(vb2.IsValid());
}
}
