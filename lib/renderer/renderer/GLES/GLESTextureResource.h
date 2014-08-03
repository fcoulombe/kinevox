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
#include "renderer/GLES/GLESPixelBufferHAL.h"
#include "renderer/RenderPipe.h"

namespace GCL
{
	class TextureResource;
	class GLESTextureResource : public GPUResource
	{
	public:
		GLESTextureResource(const TextureResource &resource);
		~GLESTextureResource();
		bool IsValid() const 
		{
			bool ret;
			RenderPipe::SendCommandSync([&](){
				ret = IsValidUnsafe(); 
			});
			return ret;
		}
		GLuint GetTextureObject() const
		{ 
			GLuint ret;
			RenderPipe::SendCommandSync([&](){
				ret = GetTextureObjectUnsafe(); 
			});
			return ret;
		}
		void Initialize(const TextureResource &imageData);
		uint32_t GetTextureUnit() const 
		{ 
			uint32_t ret;
			RenderPipe::SendCommandSync([&](){
				ret = GetTextureUnitUnsafe(); 
			});
			return ret; 
		}
		uint32_t GetTextureId() const 
		{ 
			uint32_t ret;
			RenderPipe::SendCommandSync([&](){
				ret = GetTextureIdUnsafe(); 
			});
			return ret; 
		}
		void Bind() const
		{
			RenderPipe::SendCommand([&](){
				GCLAssert(IsValidUnsafe());
			glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();
			});
		}

		size_t GetWidth() const 
		{
			GLint width;
			GCLAssert(false && "Unsupported");
			return  (size_t)width; 
		}
		size_t GetHeight() const 
		{ 
			GLint height;
			GCLAssert(false && "Unsupported");
			return  (size_t)height; 
		}
		size_t GetBytesPerPixel() const 
		{ 
			GLint format;
			GCLAssert(false && "Unsupported");
			return format;
		}
		const uint8_t *GetTextureFromVRAM() const;
		const uint8_t *GetPixelBufferFromVRAM() const;
	private:
		friend class GLESTexture;
		GLESTextureResource() {}
		GLuint mTextureId;
		GLuint mTextureUnit;
		GLESPixelBufferHAL *mPBO;

		bool IsValidUnsafe() const { return (int)mTextureId!=-1; }
		GLuint GetTextureObjectUnsafe() const { return mTextureId; }
		uint32_t GetTextureUnitUnsafe() const { return mTextureUnit; }
		uint32_t GetTextureIdUnsafe() const { return mTextureId; }
	};
}
