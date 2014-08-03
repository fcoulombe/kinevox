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
#include <gcl/Macro.h>
#include <gcl/PixelBuffer.h>
#include "renderer/GLES/GLESTextureResource.h"
#include "renderer/GLES/GLESTextureResourceManager.h"
#include "renderer/TextureResource.h"

namespace GCL
{
class GLESTexture
{
public:
	GLESTexture(const TextureResource &imageData)
	{
		mResource = static_cast<const GLESTextureResource*>(GLESTextureResourceManager::Instance().LoadResource(&imageData));
    }
	~GLESTexture()
	{
		GLESTextureResourceManager::Instance().ReleaseResource(mResource);
	}
	void Bind() const { mResource->Bind(); 	}
	size_t GetWidth() const {return mResource->GetWidth();}
	size_t GetHeight() const {return mResource->GetHeight(); }
    size_t GetBytesPerPixel() const  { 	return mResource->GetBytesPerPixel();	}
	uint32_t GetTextureUnit() const { return mResource->GetTextureUnit(); }
	uint32_t GetTextureId() const { return mResource->GetTextureId(); }
	bool IsValid() const { return mResource->IsValid(); }

	const uint8_t *GetTextureFromVRAM() const { return mResource->GetTextureFromVRAM(); }
	const uint8_t *GetPixelBufferFromVRAM() const { return mResource->GetPixelBufferFromVRAM(); }
private:
	friend class GLESGPUProgram;
	friend class GLESFrameBuffer;
	uint32_t GetTextureIdUnsafe() const { return mResource->GetTextureIdUnsafe(); }
	uint32_t GetTextureUnitUnsafe() const { return mResource->GetTextureUnitUnsafe(); }
	const GLESTextureResource *mResource;
};


}
