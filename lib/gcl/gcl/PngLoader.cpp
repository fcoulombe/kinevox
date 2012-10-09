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

#ifndef OS_IPHONE
#include <png.h>
#endif


using namespace GCL;

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
		unsigned int q = (imgHeight - i - 1) * stride;
		rowPtrs[i] = (png_bytep)data + q;
	}

	png_read_image(pngPtr, rowPtrs);

	textureData.imageData = (GLubyte *)data;

	delete [] rowPtrs;
	//And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
}
#endif

