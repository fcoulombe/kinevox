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

#include "renderer/Texture.h"

#include <cstring>
#include <stdint.h>

#include "renderer/PixelBufferHAL.h"
#include <gcl/SafeCast.h>

using namespace GCL;




GLESTexture::~GLESTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
    glDeleteTextures(1, &mTextureId); glErrorCheck();
}



void GLESTexture::Initialize(const PixelBuffer &data )
{
    glGenTextures(1, &mTextureId); glErrorCheck();
    glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();

    GCLAssert(BytesPerPixel[data.mBytesPerPixel-1]<= GL_RGBA);

#if defined(ES1)
    glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[data.mBytesPerPixel-1],
        (GLsizei)data.mWidth, (GLsizei)data.mHeight, 0,BytesPerPixel[data.mBytesPerPixel-1],
        GL_UNSIGNED_BYTE, data.mPixels);glErrorCheck();
    mPBO = new PixelBufferHAL(data);
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



const uint8_t *GLESTexture::GetPixelBufferFromVRAM() const
{
    mPBO->Bind();
    uint8_t *buffer = mPBO->PullData();
    mPBO->UnBind();
    return buffer;
}

const uint8_t *GLESTexture::GetTextureFromVRAM() const
{
    //GCLAssert(false && "unsupported");
    return NULL;
}


