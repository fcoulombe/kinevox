/*
 * Copyright (C) 2014 by Francois Coulombe
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
#include "renderer/GPUResource.h"
#include "renderer/GL/GLPixelBufferHAL.h"

namespace GCL
{
	class TextureResource;
	class GLTextureResource : public GPUResource
	{
	public:
		GLTextureResource(const TextureResource &resource);
		~GLTextureResource();
		bool IsValid() const { return (int)mTextureId!=-1; }
		GLuint GetTextureObject() const { return mTextureId; }
		void Initialize(const TextureResource &imageData);
		uint32_t GetTextureUnit() const { return mTextureUnit; }
		uint32_t GetTextureId() const { return mTextureId; }
		void Bind() const
		{
			//std::cout << "Binding Texture: " <<mTextureId << std::endl;
			GCLAssert(IsValid());
			glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();
			//mPBO->Bind();
		}

		size_t GetWidth() const 
		{
			GLint width;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			return  (size_t)width; 
		}
		size_t GetHeight() const 
		{ 
			GLint height;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
			return  (size_t)height; 
		}
		size_t GetBytesPerPixel() const 
		{ 
			GLint format;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
			return (format == GL_RGB8)?3:4;
		}
		const uint8_t *GetTextureFromVRAM() const;
		const uint8_t *GetPixelBufferFromVRAM() const;
	private:
		GLTextureResource() {}
		GLuint mTextureId;
		GLuint mTextureUnit;
		GLPixelBufferHAL *mPBO;
	};
}
