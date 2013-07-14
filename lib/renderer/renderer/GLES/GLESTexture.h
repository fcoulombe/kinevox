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
#include <3rdparty/OpenGL.h>
#include <gcl/Macro.h>
#include <gcl/PixelBuffer.h>
#include "renderer/GLES/GLESPixelBufferHAL.h"

namespace GCL
{


class GLESTexture
{
public:
	GLESTexture(const PixelBuffer &imageData)
	: mTextureId((GLuint)-1),
	  mTextureUnit((GLuint)0)
	{
        Initialize(imageData);
    }
	~GLESTexture();
	void Bind() const
	{
		//std::cout << "Binding Texture: " <<mTextureId << std::endl;
		GCLAssert(IsValid());
		glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();
		//mPBO->Bind();
	}

	size_t GetWidth() const 
	{
		GCLAssert(false && "unsupported");
		return  0; 
	}
	size_t GetHeight() const 
	{ 

		GCLAssert(false && "unsupported");
		return  (size_t)0; 
	}
    size_t GetBytesPerPixel() const 
	{ 

		GCLAssert(false && "unsupported");
		return 0;
	}
	void Initialize(const PixelBuffer &imageData);
	bool IsValid() const { return (int)mTextureId!=-1; }

    const uint8_t *GetTextureFromVRAM() const;
    const uint8_t *GetPixelBufferFromVRAM() const;

    uint32_t GetTextureUnit() const { return mTextureUnit; }
    uint32_t GetTextureId() const { return mTextureId; }
private:
	GLuint mTextureId;
	GLuint mTextureUnit;
	GLESPixelBufferHAL *mPBO;
};


}
