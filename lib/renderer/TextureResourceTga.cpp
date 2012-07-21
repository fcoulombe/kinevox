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

#include "renderer/TextureResource.h"


#include "gcl/Assert.h"
#include "renderer/TGALoaderData.h"

#include <cstring>
#include <iostream>

using namespace GCL;


void TextureResource::LoadTga(std::istream& fp, TextureData &data)
{
	TGAHeader header ;
	memset(&header, 0, sizeof(header));
	fp.seekg (0, std::ios::beg);
	fp.read((char*)&header, sizeof(TGAHeader));
	GCLAssert(fp.good());

	if(memcmp(TGAUncompressedHeader, &header, sizeof(TGAHeader)) == 0)
	{
		TGA tga;
		fp.read((char*)&tga.header, sizeof(tga.header));

		data.mWidth       = tga.width         = tga.header[1] * 256 + tga.header[0];
		data.mHeight      = tga.height        = tga.header[3] * 256 + tga.header[2];
		size_t bitPerPixel = tga.bpp           = tga.header[4];
		data.mBitdepth = 8;
		data.mBytePerPixel = tga.bytesPerPixel = bitPerPixel/data.mBitdepth;

		//test that we have power rof 2 textture
		if ( (data.mWidth & (data.mWidth - 1))  != 0  &&
				(data.mHeight & (data.mHeight - 1)) != 0 )
		{
			std::cout << "Warning: " <<  "the texture is not a power of 2" << std::endl;
		}
		if(data.mBytePerPixel == 3)
			data.mTextureFormat      = GL_RGB;
		else if (data.mBytePerPixel == 4)
			data.mTextureFormat      = GL_RGBA;

		tga.imageSize = (tga.bytesPerPixel * tga.width * tga.height);
		data.imageData =  new GLubyte[tga.imageSize];
		fp.read((char*)data.imageData, tga.imageSize);

		//swap the R and the B
		for(GLuint cswap = 0; cswap < tga.imageSize; cswap += tga.bytesPerPixel)
		{
			uint8_t temp = data.imageData[cswap];
			data.imageData[cswap] = data.imageData[cswap+2];
			data.imageData[cswap+2] = temp;
		}
	}
	//check if header is compressed
	else if(memcmp(TGACompressedHeader, &header, sizeof(TGAHeader)) == 0)
	{
		TGA tga;
		fp.read((char*)&tga.header, sizeof(tga.header));

		data.mWidth  = tga.width = tga.header[1] * 256 + tga.header[0];
		data.mHeight = tga.height = tga.header[3] * 256 + tga.header[2];
		size_t bitPerPixel  = tga.bpp = tga.header[4];
		data.mBitdepth = 8;
		data.mBytePerPixel = tga.bytesPerPixel = bitPerPixel/data.mBitdepth;

		GCLAssert(data.mWidth >= 0 &&
				data.mHeight >= 0 &&
				(data.mBytePerPixel == 3 || data.mBytePerPixel ==4)) ;

		if(data.mBytePerPixel == 3)           // Is It A 24bpp Image?
			data.mTextureFormat    = GL_RGB;   // If So, Set Type To GL_RGB
		else                    // If It's Not 24, It Must Be 32
			data.mTextureFormat    = GL_RGBA;  // So Set The Type To GL_RGBA

		tga.bytesPerPixel   = (tga.bpp / 8);
		tga.imageSize       = (tga.bytesPerPixel * tga.width * tga.height);

		// Allocate Memory To Store Image Data
		data.imageData   = new GLubyte[tga.imageSize];

		GLuint pixelcount = tga.height * tga.width; // Number Of Pixels In The Image
		GLuint currentpixel = 0;            // Current Pixel We Are Reading From Data
		GLuint currentbyte  = 0;            // Current Byte We Are Writing Into Imagedata
		// Storage For 1 Pixel
		GLubyte * colorbuffer = new GLubyte[tga.bytesPerPixel];

		do
		{
			GLubyte chunkheader = 0;            // Variable To Store The Value Of The Id Chunk
			fp.read((char*)&chunkheader, sizeof(GLubyte));

			if(chunkheader < 128)                // If The Chunk Is A 'RAW' Chunk
			{
				chunkheader++;              // Add 1 To The Value To Get Total Number Of Raw Pixels
				// Start Pixel Reading Loop
				for(size_t counter = 0; counter < chunkheader; counter++)
				{
					// Try To Read 1 Pixel
					fp.read((char*)colorbuffer, tga.bytesPerPixel) ;
					//GCLAssert(fp.good());
					GLubyte &temp = data.imageData[currentbyte];
					GLubyte temp2 = colorbuffer[2];
					temp = temp2;
					data.imageData[currentbyte] = colorbuffer[2];        // Write The 'R' Byte
					data.imageData[currentbyte + 1   ] = colorbuffer[1]; // Write The 'G' Byte
					data.imageData[currentbyte + 2   ] = colorbuffer[0]; // Write The 'B' Byte
					if(tga.bytesPerPixel == 4)                  // If It's A 32bpp Image...
					{
						data.imageData[currentbyte + 3] = colorbuffer[3];    // Write The 'A' Byte
					}
					// Increment The Byte Counter By The Number Of Bytes In A Pixel
					currentbyte += tga.bytesPerPixel;
					currentpixel++;                 // Increment The Number Of Pixels By 1
				}
			}
			else                        // If It's An RLE Header
			{
				chunkheader -= 127;         // Subtract 127 To Get Rid Of The ID Bit
				// Read The Next Pixel
				fp.read((char*)colorbuffer, tga.bytesPerPixel) ;

				// Start The Loop
				for(size_t counter = 0; counter < chunkheader; counter++)
				{
					// Copy The 'R' Byte
					data.imageData[currentbyte] = colorbuffer[2];
					// Copy The 'G' Byte
					data.imageData[currentbyte + 1   ] = colorbuffer[1];
					// Copy The 'B' Byte
					data.imageData[currentbyte + 2   ] = colorbuffer[0];
					if(tga.bytesPerPixel == 4)      // If It's A 32bpp Image
					{
						// Copy The 'A' Byte
						data.imageData[currentbyte + 3] = colorbuffer[3];
					}
					currentbyte += tga.bytesPerPixel;   // Increment The Byte Counter
					currentpixel++;             // Increment The Pixel Counter
				}
			}
		}	while(currentpixel < pixelcount);    // More Pixels To Read? ... Start Loop Over
		delete [] colorbuffer;

	}
	//unknown header
	else
	{
		data.imageData = NULL;
		GCLAssert(false && "Unknown image header");
	}
}


