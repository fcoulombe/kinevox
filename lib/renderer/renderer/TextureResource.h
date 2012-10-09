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
#include <3rdparty/OpenGL.h>
#include <gcl/Resource.h>
#include <gcl/PixelBuffer.h>

namespace GCL
{
  //this is a resource class that can be shared between difference texture instance
  class TextureResource : public Resource
  {
    public:

    TextureResource(const char *textureName);
    ~TextureResource();

    struct TextureData
    {
      TextureData()
      : mTextureHandle((GLuint)-1),
          mMinFilter(GL_LINEAR),
          mMagFilter(GL_LINEAR),
          //mWrapModeS(GL_CLAMP),
          //mWrapModeT(GL_CLAMP)
          mWrapModeS(GL_CLAMP_TO_EDGE),
          mWrapModeT(GL_CLAMP_TO_EDGE),
          mTextureFormat(GL_RGB)
      {
      }
      ~TextureData()
      {

      }

      PixelBuffer imageData;
      GLuint mTextureHandle;
      GLenum mMinFilter;
      GLenum mMagFilter;
      GLenum mWrapModeS;
      GLenum mWrapModeT;
      GLenum mTextureFormat;
 /*     GLuint mBytePerPixel;
      GLuint mBitdepth;
      GLsizei mWidth,
              mHeight;*/
    }mTextureData;

    static const TextureResource EmptyTexture;


    static void Unload(TextureData &data);
  private:
    TextureResource() {}
  };
}
