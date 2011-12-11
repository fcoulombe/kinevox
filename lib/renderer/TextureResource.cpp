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
	fp.seekg (0, std::ios::beg);
	fp.read((char*)&header, sizeof(TGAHeader));

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
		data.imageData = NULL;
		GCLAssert(false && "TBD"); //unsupported
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


	/* glGenTextures( 1, &mTextureHandle );	glErrorCheck();
  glBindTexture(	 GL_TEXTURE_2D, mTextureHandle );glErrorCheck();
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter );glErrorCheck();
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter );glErrorCheck();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapModeS );glErrorCheck();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapModeT);glErrorCheck();
  GLenum bytePerPixel = (mBytePerPixel==4) ? GL_RGBA : GL_RGB;
  glTexImage2D(	 GL_TEXTURE_2D, 0, bytePerPixel, mWidth, mHeight, 0,mTextureFormat, GL_UNSIGNED_BYTE, imageData );glErrorCheck();

  delete [] imageData;*/
}

TextureResource::~TextureResource()
{
	if (mTextureData.imageData)
	{
		delete [] mTextureData.imageData;
	}

	//glDeleteTextures(1, &mTextureHandle);	glErrorCheck();
}

