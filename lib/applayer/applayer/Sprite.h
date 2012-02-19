#pragma once


#include "renderer/Material.h"

namespace GCL
{


struct SpriteData
{
	uint16_t width;
	uint16_t height;
	uint16_t frameCount;
	uint8_t pad[2];
	const char *filename;
};


class Sprite
{
public:
	Sprite(const char *filename="DefaultSprite");
private:
	void LoadSprite(const char *filename);
	Material mMaterial;
	SpriteData mData;
};

}
