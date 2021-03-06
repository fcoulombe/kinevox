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
#include <vector>
#include <3rdparty/OpenRL.h>

namespace GCL
{
class Matrix44;
class RLTexture;
class RLShader;
  class RLProgram
  {
  public:
    RLProgram();
    ~RLProgram();
    void Bind();
    bool IsValid() const { return mIsValid; }
    void AttachShader(RLShader &shader);
    void Link();

    void SetTextureSampler(const RLTexture &sampler);
    void SetProjectionMatrix(const Matrix44 &m);
    void SetModelViewMatrix(const Matrix44 &m);
    void GetUniform(const char *unformName, Matrix44 &m44) const;
    void GetUniform(const char *unformName, int &ret) const;
    int GetAttributeLocation(const char *attributeName) const;

    static void ResetDefault();
  private:
    void PrintInfoLog(RLprogram );
 
    std::vector<RLShader *> mShaderList;
    RLprogram mProgramObject;
    bool mIsValid;
  };
}
