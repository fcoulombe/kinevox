#include "renderer/Texture.h"

#include <cstring>
#include <stdint.h>

#include <gcl/SafeCast.h>
#include "renderer/TextureResource.h"
#include "renderer/TextureResourceManager.h"



using namespace GCL;


Texture::Texture(const char *filename)
: mTextureId(-1)
{
	bool ret = LoadTexture(filename);
	GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

	const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
	Initialize(tempTextureData.mWidth, tempTextureData.mHeight, tempTextureData.mBytePerPixel, tempTextureData.imageData);
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
	mTextureData.width = width;
	mTextureData.height= height;
	mTextureData.bytesPerPixel = bytesPerPixel;
	Initialize(width, height, bytesPerPixel);
}
static const GLenum BytePerPixel[] =
{
	GL_LUMINANCE,
	GL_LUMINANCE,
	GL_RGB,
	GL_RGBA
};
static const GLenum BytesPerPixel[] =
{
    GL_LUMINANCE,
    GL_LUMINANCE,
    GL_RGB,
    GL_RGBA,
};
void Texture::Initialize(size_t width, size_t height, size_t bytesPerPixel, const uint8_t *data )
{
	glGenTextures(1, &mTextureId); glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();
	GCLAssert(bytesPerPixel <= GL_RGBA);
	/*glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
			BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();*/
#ifndef ES1
	if (data)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
#endif
		glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
					BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, 0);glErrorCheck();
}

bool Texture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);


	mTextureData.width = mTextureResource->mTextureData.mWidth;
	mTextureData.height= mTextureResource->mTextureData.mHeight;
	mTextureData.bytesPerPixel = mTextureResource->mTextureData.mBytePerPixel;

	return mTextureResource != 0;
}



const uint8_t *Texture::GetTextureFromVRAM() const
{
#if !defined(ES1) && !defined(ES2)
	Bind();
	long imageSize = mTextureData.GetImageSizeInBytes();
	uint8_t *data = new uint8_t[imageSize];

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    
	return data;
#else
    GCLAssert(false && "unsupported");
    return NULL;
#endif
}

#include <fstream>
//TODO: push this in the texture resource
void Texture::Save(const char *filename)
{
	const uint8_t *data = GetTextureFromVRAM();
	const char *ext = &(filename[strlen(filename)-3]);
	if (strncmp(ext, "tga", 3)==0)
	{
		uint16_t x = mTextureData.width;
		uint16_t y = mTextureData.height;
		// split x and y sizes into bytes

		//assemble the header
		uint8_t header[18]={
				0, //1			1
				0, //2 garbage	2
				2, //1 type		3
				0, //1			4
				0, //2			5
				0, //3			6
				0, //4			7
				0, //5 garbage	8
				0, //1			9
				0, //2 xstart	10
				0, //1			11
				0, //2 ystart	12
				0, //1 			13
				0, //2 width	14
				0, //1			15
				0, //2 height	16
				0, //1 bpp		17
				0, //1 desc		18
				};

		*(uint16_t*)&(header[8]) = 0;
		*(uint16_t*)&(header[10]) = 0;
		*(uint16_t*)&(header[12]) = x;
		*(uint16_t*)&(header[14]) = y;
		*(uint8_t*)&(header[16]) = mTextureData.bytesPerPixel*8;

		// write header and data to file
		std::fstream File(filename, std::ios::out | std::ios::binary);
		File.write ((const char *)&header, sizeof (uint8_t)*18);
		File.write ((const char *)data, sizeof (uint8_t)*mTextureData.GetImageSizeInBytes()     );
		File.close();

		delete[] data;
		data=NULL;
		//SaveTga(filename);
	}
	else if (strncmp(ext, "png", 3)==0)
	{
		GCLAssert(false && "TBD");
		//SavePng(filename);
	}
	else
	{
		std::string s("This extension is not supported: ");
		s += ext;
		GCLAssertMsg(false, s)
	}
}
