#include "renderer/Sprite.h"
#include <gcl/Assert.h>
#include <renderer/ResourceManagerConfig.h>
#include <renderer/Texture.h>

#include <fstream>

using namespace GCL;


Sprite::Sprite(const char *filename)
: mCurrentFrame(0),
  mIsPlaying(false)
{
	LoadSprite(filename);
}

void Sprite::LoadSprite(const char * filename)
{
	const std::string fullFileName(std::string(SPRITE_PATH) + std::string(filename) + std::string(".spr"));

	std::fstream fp(fullFileName, std::fstream::binary|std::fstream::in);
	GCLAssertMsg(fp.good(), fullFileName);
	fp.read((char*)&mHeader, sizeof(SpriteDataHeader));
	std::cout << "width: " << mHeader.width<<std::endl;
	std::cout << "height: " << mHeader.height<<std::endl;
	std::cout << "frame count: " << mHeader.frameCount<<std::endl;
	std::cout << "texture count: " << mHeader.textureCount<<std::endl;
	for (size_t i=0; i<mHeader.textureCount; ++i)
	{
		uint32_t strLen;
		fp.read((char*)&strLen, sizeof(uint32_t));

		char *buffer = (char*)alloca(strLen+1);
		fp.read(buffer, strLen+1);

		std::string fullFileName(TEXTURE_PATH);
		fullFileName += buffer;
		mTextureList.push_back(new Texture(fullFileName.c_str()));
	}
	fp.close();
}


void Sprite::Play()
{
	mIsPlaying = true;
}

void Sprite::Pause()
{
	mIsPlaying = false;
}

void Sprite::Rewind()
{
	mCurrentFrame = 0;
}


void Sprite::Update()
{

}
