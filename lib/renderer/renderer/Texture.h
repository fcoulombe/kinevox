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

#define TEXTURE_INCLUDE GET_GFX_INCLUDE(Texture.h)
#define ITexture GET_GFX_CLASS(Texture)
#include TEXTURE_INCLUDE


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


	void Bind() const { mPimpl->Bind(); }
	bool IsValid() const { return mPimpl->IsValid(); }
	size_t GetWidth() const { return mPimpl->GetWidth(); }
	size_t GetHeight() const { return mPimpl->GetHeight(); }
    size_t GetBytesPerPixel() const { return mPimpl->GetBytesPerPixel(); }


    const uint8_t *GetTextureFromVRAM() const { return mPimpl->GetTextureFromVRAM(); }
    const uint8_t *GetPixelBufferFromVRAM() const { return mPimpl->GetPixelBufferFromVRAM(); }
private:
    const ITexture &GetImpl() const { return *mPimpl; }
	ITexture *mPimpl;
    friend class FrameBuffer;
    friend class Shader;
	const TextureResource *mTextureResource;
};

}
