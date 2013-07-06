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

#include "renderer/GL/GLTexture.h"

#include <cstring>
#include <stdint.h>

#include "renderer/PixelBufferHAL.h"
#include <gcl/SafeCast.h>

using namespace GCL;

GLTexture::~GLTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
    glDeleteTextures(1, &mTextureId); glErrorCheck();
}

void GLTexture::Initialize(const PixelBuffer &data )
{
    glGenTextures(1, &mTextureId); glErrorCheck();
    glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
#if ENABLE_FIX_PIPELINE 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();
#endif

    GCLAssert(BytesPerPixel[data.mBytesPerPixel-1]<= GL_RGBA);

#if 0
    glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[data.mBytesPerPixel-1],
        data.mWidth, data.mHeight, 0,BytesPerPixel[data.mBytesPerPixel-1],
        GL_UNSIGNED_BYTE, data.mPixels);glErrorCheck();
    glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
#else
    glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[data.mBytesPerPixel-1],
        (GLsizei)data.mWidth, (GLsizei)data.mHeight, 0,BytesPerPixel[data.mBytesPerPixel-1],
        GL_UNSIGNED_BYTE, NULL);glErrorCheck();

    mPBO = new PixelBufferHAL(data);
    if (data.mPixels)
    {
        mPBO->Bind();
        mPBO->PushData();
        //push from pbo to texture
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)mPBO->mWidth,
            (GLsizei)mPBO->mHeight, BytesPerPixel[mPBO->mBytesPerPixel-1],
            GL_UNSIGNED_BYTE, 0);glErrorCheck();
        mPBO->UnBind();
    }
    glGenerateMipmap(GL_TEXTURE_2D);glErrorCheck();
#endif
}



const uint8_t *GLTexture::GetPixelBufferFromVRAM() const
{
    mPBO->Bind();
    uint8_t *buffer = mPBO->PullData();
    mPBO->UnBind();
    return buffer;
}

const uint8_t *GLTexture::GetTextureFromVRAM() const
{
#if !defined(ES1) && !defined(ES2)
    Bind();
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    size_t imageSize = width*height*mPBO->mBytesPerPixel;

    uint8_t *data = new uint8_t[imageSize];
    memset(data, 0, imageSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);glErrorCheck();

    if (mPBO->mBytesPerPixel == 3)
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (mPBO->mBytesPerPixel == 4)
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
    {
        GCLAssert(false);
    }
    glErrorCheck();
    return data;
#else
    GCLAssert(false && "unsupported");
    return NULL;
#endif
}


