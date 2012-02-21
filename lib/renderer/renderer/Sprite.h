#pragma once

#include <vector>

#include "renderer/Material.h"

namespace GCL
{
struct SpriteDataHeader
{
	uint16_t width;
	uint16_t height;
	uint32_t frameCount;
	uint32_t textureCount;
};

class Texture;
class Sprite
{
public:
	Sprite(const char *filename="DefaultSprite");

	void Render() const;

	void Update();
	void Play();
	void Pause();
	void Rewind();

	size_t GetWidth() const { return mHeader.width; }
	size_t GetHeight() const { return mHeader.height; }
	size_t GetFrameCount() const { return mHeader.frameCount; }

	void SetPosition(const WorldPoint3 &position)  	{mPosition = position;	}
private:
	void LoadSprite(const char *filename);
	std::vector<Texture*> mTextureList;

	SpriteDataHeader mHeader;

	size_t mCurrentFrame;
	WorldPoint3 mPosition;
	bool mIsPlaying;
};

}
