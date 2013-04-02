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
#include <3rdparty/OpenRL.h>
#include <gcl/Macro.h>
#include "renderer/RLPixelBufferHAL.h"

namespace GCL
{
class TextureResource;
class PixelBuffer;

struct RLTextureData
{
	size_t width, height, bytesPerPixel;
	size_t GetImageSizeInBytes() const { return width*height*bytesPerPixel; }
};

class RLTexture
{
public:
	RLTexture(const PixelBuffer &buffer);
	RLTexture(const char *filename);
	RLTexture(size_t width, size_t height, size_t bypesPerPixel = 4);
	~RLTexture();
	bool LoadTexture(const char *filename);

	void Bind() const
	{
		//std::cout << "Binding Texture: " <<mTextureId << std::endl;
		GCLAssert(IsValid());
		rlBindTexture(RL_TEXTURE_2D, mTextureId);  
		//mPBO->Bind();
	}

	bool IsValid() const { return (int)mTextureId!=-1; }

	void Save(const char *filename);

	size_t GetWidth() const { return mPBO->mWidth; }
	size_t GetHeight() const { return mPBO->mHeight; }


	static void ResetDefault() { GCLAssert(false); }

private:
	void Initialize(const PixelBuffer &imageData);
	const uint8_t *GetTextureFromVRAM() const;
	const uint8_t *GetPixelBufferFromVRAM() const;


	friend class RLProgram;
	friend class RLFrameBuffer;
    RLuint GetTextureUnit() const { GCLAssert(false); return (RLuint)-1; }
	RLtexture  GetTextureId() const { return mTextureId; }
	RLtexture mTextureId;
	RLTextureData mTextureData;

	RLPixelBufferHAL *mPBO;
	const TextureResource *mTextureResource;
};

GCLINLINE std::ostream& operator<<( std::ostream& output, const RLTextureData &P)
{
	output << "(width: " << P.width << ", height: " << P.height << ", bpp: " << P.bytesPerPixel << ")" << std::endl;
	return output;
}

}
