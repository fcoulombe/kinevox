/*
 * Copyright (C) 2013 by Francois Coulombe
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
#define SHADER_INCLUDE GET_GFX_INCLUDE(Shader.h)
#define IShader GET_GFX_CLASS(Shader)
#include SHADER_INCLUDE
#include "renderer/Texture.h"
namespace GCL
{
class Matrix44;
class Texture;
  class Shader
  {
  public:
    Shader() { }

    ~Shader() {}
    void Bind() { mPimpl.Bind(); }
    bool IsValid() const { return mPimpl.IsValid(); }

    void SetTextureSampler(const Texture &sampler) { mPimpl.SetTextureSampler(sampler.GetImpl()); }
    void SetProjectionMatrix(const Matrix44 &m) { mPimpl.SetProjectionMatrix(m); }
    void SetModelViewMatrix(const Matrix44 &m) { mPimpl.SetModelViewMatrix(m); }
    void GetUniform(const char *unformName, Matrix44 &m44) const { mPimpl.GetUniform(unformName, m44); }
    void GetUniform(const char *unformName, int &ret) const { mPimpl.GetUniform(unformName, ret); }
    int GetAttributeLocation(const char *attributeName) const { return mPimpl.GetAttributeLocation(attributeName); }

    static void ResetDefault() { IShader::ResetDefault(); }
  private:
    IShader mPimpl;
  };
}
