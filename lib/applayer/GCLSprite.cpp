#include "applayer/GCLSprite.h"
#include "applayer/GCLApplication.h"

using namespace GCL;

GCLSprite::GCLSprite(const char *filename)
: Sprite(filename)
{
	GCLApplication::RegisterSprite(this);
}

GCLSprite::~GCLSprite()
{
	GCLApplication::ReleaseSprite(this);
}
