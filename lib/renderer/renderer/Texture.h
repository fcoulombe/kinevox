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
#include <gcl/Macro.h>
#include "rendererconf.h"
#include GFXAPI_Texture_H
namespace GCL
{
class TextureResource;
class PixelBuffer;

class Texture
{
public:
	Texture(const PixelBuffer &buffer);
	Texture(const char *filename);
	Texture(size_t width, size_t height, size_t bypesPerPixel = 4);
	~Texture();
	bool LoadTexture(const char *filename);

	void Save(const char *filename);


	void Bind() const { mTexture->Bind(); }
	bool IsValid() const { return mTexture->IsValid(); } 
	
	size_t GetResourceWidth() const;
	size_t GetResourceHeight() const;
	size_t GetResourceBytesPerPixel() const;

	size_t GetWidth() const { return mTexture->GetWidth(); } 
	size_t GetHeight() const {return mTexture->GetHeight(); } 
    size_t GetBytesPerPixel() const { return mTexture->GetBytesPerPixel(); } 

    const uint8_t *GetTextureFromVRAM() const { return mTexture->GetTextureFromVRAM(); }
    const uint8_t *GetPixelBufferFromVRAM() const { return mTexture->GetPixelBufferFromVRAM(); } 
private:
	void Create();
	const TextureResource *mTextureResource;
	ITexture *mTexture;
	friend class GPUProgram;
	friend class FrameBuffer;
};

}
