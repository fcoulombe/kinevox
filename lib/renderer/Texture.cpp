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
	Initialize(width, height, bytesPerPixel);
}

void Texture::Initialize(size_t width, size_t height, size_t bytesPerPixel, const uint8_t *data )
{
    std::cout << "tex: " << width << " " << height << " " << bytesPerPixel << std::endl;
	mTextureData.width = width;
	mTextureData.height= height;
	mTextureData.bytesPerPixel = bytesPerPixel;

	glGenTextures(1, &mTextureId); glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); glErrorCheck();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);glErrorCheck();
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmapglErrorCheck();

	GLenum BytePerPixel[] =
	{
		GL_LUMINANCE,
		GL_LUMINANCE16,
		GL_RGB8,
		GL_RGBA8
	};
	GLenum BytesPerPixel[] =
	{
	    GL_LUMINANCE,
	    GL_LUMINANCE,
	    GL_RGB,
	    GL_RGBA,
	};
	GCLAssert(bytesPerPixel <= GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, BytePerPixel[bytesPerPixel-1], width, height, 0,
			BytesPerPixel[bytesPerPixel-1], GL_UNSIGNED_BYTE, data);glErrorCheck();
	glBindTexture(GL_TEXTURE_2D, 0);glErrorCheck();
}

bool Texture::LoadTexture(const char *filename)
{
	const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
	mTextureResource = static_cast<const TextureResource*>(tempResource);

	return mTextureResource != 0;
}



const uint8_t *Texture::GetTextureFromVRAM() const
{
	Bind();
	long imageSize = mTextureData.GetImageSizeInBytes();
	uint8_t *data = new uint8_t[imageSize];
	glReadPixels(0,0,mTextureData.width,mTextureData.height, GL_BGR,GL_UNSIGNED_BYTE,data);glErrorCheck();
	return data;
	//glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0, mTextureData.width,mTextureData.height,0);
}

#include <fstream>
void Texture::Save(const char *filename)
{
	const uint8_t *data = GetTextureFromVRAM();
	const char *ext = &(filename[strlen(filename)-3]);
	if (strncmp(ext, "tga", 3)==0)
	{
		int x = mTextureData.width;
		int y = mTextureData.height;
		// split x and y sizes into bytes
		int xa= x % 256;
		int xb= (x-xa)/256;

		int ya= y % 256;
		int yb= (y-ya)/256;

		//assemble the header
		const uint8_t header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(uint8_t)xa,(uint8_t)xb,(uint8_t)ya,(uint8_t)yb,24,0};

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
