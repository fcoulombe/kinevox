#include "renderer/TextureResourceManager.h"
#include "renderer/TextureResource.h"

using namespace GCL;

TextureResourceManager *TextureResourceManager::smpInstance = NULL;

Resource * TextureResourceManager::Allocate( const char *filename )
{
  return new TextureResource(filename);
}

void TextureResourceManager::Free( Resource * /*resource*/ )
{

}
