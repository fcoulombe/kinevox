#pragma once

#include <cstdlib>


namespace GCL
{
class ViewPort
{
public:
	static const size_t DEFAULT_SCREEN_WIDTH;
	static const size_t DEFAULT_SCREEN_HEIGHT ;

	ViewPort()
	: mWidth(DEFAULT_SCREEN_WIDTH), mHeight(DEFAULT_SCREEN_HEIGHT) {}

	size_t GetWidth() const { return mWidth; }
	size_t GetHeight() const { return mHeight; }

private:
	size_t mWidth;
	size_t mHeight;
};

}
