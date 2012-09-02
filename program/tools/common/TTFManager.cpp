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

#include "common/TTFManager.h"
#include <gcl/Assert.h>
#include <gcl/PixelBuffer.h>
#include <renderer/TextureResource.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace GCL;
FT_Library  TTFManager::library;

void TTFManager::Initialize()
{
	FT_Error error = FT_Init_FreeType( &library );
	FTErrorCheck(error);
}


void TTFManager::Terminate()
{
	FT_Error error = FT_Done_FreeType(library);
	FTErrorCheck(error);
}

void GenerateAsciiFont(const FT_Face &face)
{
	const size_t kFontSize = 16;
	/*const char *alphabet = " !\"#$%&'()*+,-./0123456789:;<=>?@"
							"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
							"abcdefghijklmnopqrstuvwxyz{|}~";
*/
	size_t numChars = 255;//strlen(alphabet);
	/*size_t areaSquare = kFontSize * kFontSize*numChars;

	std::cout << "area: " << areaSquare << std::endl;
	std::cout << "32: " << 32*32 << std::endl;
	std::cout << "64: " << 64*64 << std::endl;
	std::cout << "128: " << 128*128 << std::endl;
	std::cout << "256: " << 256*256 << std::endl;
	std::cout << "512: " << 512*512 << std::endl;
*/

	const size_t imageSize = 256;
	const size_t numChannel = 4;

	std::vector<uint8_t*> charArray;
	for (size_t c =0; c<numChars; ++c)
	{
		FT_UInt glyph_index = FT_Get_Char_Index( face, c);//alphabet[c]);

		FT_Error error = FT_Load_Glyph(
				face,
				glyph_index,
				FT_LOAD_DEFAULT );
		FTErrorCheck(error);

		error = FT_Render_Glyph( face->glyph,
				FT_RENDER_MODE_NORMAL );
		FTErrorCheck(error);

		FT_Bitmap  &bitmap = face->glyph->bitmap;

		uint8_t *glyph = new uint8_t[kFontSize*kFontSize*numChannel];
		GCLAssert((size_t)bitmap.rows<=kFontSize);
		GCLAssert((size_t)bitmap.width<=kFontSize);

		for (size_t i=0; i<(size_t)bitmap.rows; ++i)
		{
			for (size_t j=0; j<(size_t)bitmap.width; ++j)
			{
				uint8_t t = bitmap.buffer[i*bitmap.pitch + j];
				size_t y = i*kFontSize*numChannel;
				size_t x = j*numChannel;
				size_t pixelIndex = y+x;

				for (size_t k=0; k<numChannel-1; ++k)
				{
					glyph[pixelIndex+k] = 0;
				}
				glyph[pixelIndex+numChannel-1] = t;
				GCLAssert(y + x+3 <kFontSize*kFontSize*numChannel);
			}
		}
		charArray.push_back(glyph);
	}

	const size_t charPerRow = imageSize/kFontSize;
	uint8_t *buffer = new uint8_t[imageSize*imageSize*numChannel];

	//blit the fonts on the texture
	for (size_t i=0; i<charArray.size(); ++i)
	{
			uint8_t *currentChar = charArray[i];
			size_t col = i%charPerRow;
			size_t row = i/charPerRow;
			//std::cout << "Row: " << row << " col: " << col << std::endl;
			for (size_t j=0; j<kFontSize; ++j)
			{
				size_t destY = row*kFontSize*imageSize*numChannel+j*imageSize*numChannel;
				size_t destX = col*kFontSize*numChannel;
				uint8_t *dest = &(buffer[destY + destX]);
				uint8_t *src = &(currentChar[j*kFontSize*numChannel]);
				memcpy(dest, src, kFontSize*numChannel);
			}
	}

	PixelBuffer::SaveTga("glysavetest.tga",imageSize, imageSize, numChannel, buffer);
	delete [] buffer;
	for (size_t i=0; i<charArray.size(); ++i)
	{
		delete [] charArray[i];
	}
	charArray.clear();
}

bool TTFManager::LoadScene(const char* pFilename)
{
	FT_Face     face;
	FT_Error error = FT_New_Face( library,
			pFilename,
			0,
			&face );
	FTErrorCheck(error);
	/*error = FT_Select_CharMap(
	            face,
	            FT_ENCODING_NONE );
	FTErrorCheck(error);
	 */
	error = FT_Set_Pixel_Sizes(
			face,
			0,      /* pixel_width           */
			16 );   /* pixel_height          */
	FTErrorCheck(error);


	GenerateAsciiFont(face);
	return false;
}
