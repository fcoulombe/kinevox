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


#include <fstream>
#include <iostream>

#include "gcl/Assert.h"
#include "renderer/TGALoaderData.h"

#ifndef OS_IPHONE
#include <png.h>
#endif


using namespace GCL;

const TextureResource TextureResource::EmptyTexture;

void TextureResource::Unload(TextureData &data)
{
	GCLAssert(data.imageData);
	delete [] data.imageData;
}


void TextureResource::LoadRaw(std::istream& fp, TextureData &data)
{
	data.mWidth = 256;
	data.mHeight = 256;
	data.mBytePerPixel = 3;
	data.mBitdepth = 8;
	size_t size = data.mWidth * data.mHeight* data.mBytePerPixel ;
	data.imageData =  new GLubyte[size];
	fp.read( (char*)data.imageData, size);
}

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

	}
	//unknown header
	else
	{
		data.imageData = NULL;
		GCLAssert(false && "Unknown image header");
	}
}

#ifndef OS_IPHONE

#define PNGSIGSIZE 8

static void validate(FILE* source)
{
	png_byte pngsig[PNGSIGSIZE];
	size_t len = fread(pngsig,1, PNGSIGSIZE,source);
	GCLAssert(len == PNGSIGSIZE);
	int is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	if (is_png != 0)
		throw GCLException("ERROR: Data is not valid PNG-data");
}

void TextureResource::LoadPng(FILE *source, TextureData &textureData)
{
	validate(source);

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
		throw GCLException("ERROR: Couldn't initialize png read struct" );

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		throw GCLException("ERROR: Couldn't initialize png info struct" );
	}


	png_bytep* rowPtrs = NULL;
	char* data = NULL;

	if (setjmp(png_jmpbuf(pngPtr))) {
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		if (rowPtrs != NULL)
			delete [] rowPtrs;
		if (data != NULL)
			delete [] data;

		throw GCLException("ERROR: An error occured while reading the PNG file");
	}

	png_init_io(pngPtr, source);

	png_set_sig_bytes(pngPtr, PNGSIGSIZE);

	png_read_info(pngPtr, infoPtr);

	png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
	textureData.mWidth = imgWidth;
	png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);
	textureData.mHeight = imgHeight;

	png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);

	png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
	png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(pngPtr);
		channels = 3;
		break;
	case PNG_COLOR_TYPE_GRAY:
		/*  if (bitdepth < 8)
      png_set_gray_1_2_4_to_8(pngPtr);*/
		bitdepth = 8;
		textureData.mBytePerPixel = 1;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		textureData.mBytePerPixel = 4;
		break;
	case PNG_COLOR_TYPE_RGB:
		textureData.mBytePerPixel = 3;
		break;
	default:
		GCLAssert(false);
	}
	textureData.mBitdepth = bitdepth;

	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		channels+=1;
	}

	//loading the data
	rowPtrs = new png_bytep[imgHeight];
	data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
	const unsigned int stride = imgWidth * bitdepth * channels / 8;


	//A little for-loop here to set all the row pointers to the starting
	//Adresses for every row in the buffer
	for (size_t i = 0; i < imgHeight; i++) {
		unsigned int q = (imgHeight- i - 1) * stride;
		rowPtrs[i] = (png_bytep)data + q;
	}

	png_read_image(pngPtr, rowPtrs);

	textureData.imageData = (GLubyte *)rowPtrs;
	//And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
}
#endif

//this loads TGA files and then upload it to opengl
TextureResource::TextureResource( const char *textureName )
: Resource()
{
	std::string path = Resource::GetResourcePath();
	path += textureName;

	std::string msg("trying to loads " );
	msg += path.c_str();
	msg += "\n";

	const char *ext = &(path.c_str()[path.length()-3]);
	TextureData &data = mTextureData;
	if (strncmp(ext, "tga", 3) == 0)
	{
		std::fstream fp(path.c_str(), std::fstream::binary|std::fstream::in);
		GCLAssertMsg(fp.is_open() && fp.good(), msg.c_str());

		LoadTga(fp, data);
		GCLAssert(data.imageData);

		fp.close();
	}


	else if (strncmp(ext, "png", 3) == 0)
	{
#ifndef OS_IPHONE
		FILE *fp = fopen(path.c_str(), "rb");
		GCLAssertMsg(fp, path.c_str());

		LoadPng(fp, data);
		GCLAssert(data.imageData);
#else
		GCLAssertMsg(false, "cant load png on iphone yet" );
#endif

	}
	else if (strncmp(ext, "raw", 3) == 0)
	{
		GCLAssert(false && "can't load a raw texture like this since we dont have information about dimensions");
	}
	else
	{
		GCLAssertMsg(false, "this extension is not supported")
	}
}

TextureResource::~TextureResource()
{
	if (mTextureData.imageData)
	{
		delete [] mTextureData.imageData;
	}
}

