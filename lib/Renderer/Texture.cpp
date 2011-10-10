#include "Renderer/Texture.h"

#include <GCL/SafeCast.h>
#include "Renderer/TextureResource.h"
#include "Renderer/TextureResourceManager.h"


#define ENABLE_DEVIL 0
#if ENABLE_DEVIL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

inline void ilErrorCheck()
{

  ILenum ret = ilGetError();
  if (ret == IL_NO_ERROR)
    return;
  switch(ret)
  {
  case IL_INVALID_ENUM:
    GCLAssertMsg(false, "An unacceptable enumerated value was passed to a function.");
    break;
  case IL_OUT_OF_MEMORY:
    GCLAssertMsg(false, "Could not allocate enough memory in an operation.");
    break;
  case IL_FORMAT_NOT_SUPPORTED:
    GCLAssertMsg(false, "The format a function tried to use was not able to be used by that function.");
    break;
  case IL_INTERNAL_ERROR:
    GCLAssertMsg(false, "A serious error has occurred. Please e-mail DooMWiz with the conditions leading up to this error being reported.");
    break;
  case IL_INVALID_VALUE:
    GCLAssertMsg(false, "An invalid value was passed to a function or was in a file.");
    break;
  case IL_ILLEGAL_OPERATION:
    GCLAssertMsg(false, "The operation attempted is not allowable in the current state. The function returns with no ill side effects.");
  case IL_ILLEGAL_FILE_VALUE:
    GCLAssertMsg(false, "An illegal value was found in a file trying to be loaded.");
    break;
  case IL_INVALID_FILE_HEADER:
    GCLAssertMsg(false, "A file's header was incorrect.");
    break;
  case IL_INVALID_PARAM:
    GCLAssertMsg(false, "An invalid parameter was passed to a function, such as a NULL pointer.");
    break;
  case IL_COULD_NOT_OPEN_FILE:
    GCLAssertMsg(false, "Could not open the file specified. The file may already be open by another app or may not exist.");
    break;
  case IL_INVALID_EXTENSION:
    GCLAssertMsg(false, "The extension of the specified filename was not correct for the type of image-loading function.");
    break;
  case IL_FILE_ALREADY_EXISTS:
    GCLAssertMsg(false, "The filename specified already belongs to another file. To overwrite files by default, call ilEnable with the IL_FILE_OVERWRITE parameter.");
    break;
  case IL_OUT_FORMAT_SAME:
    GCLAssertMsg(false, "Tried to convert an image from its format to the same format.");
    break;
  case IL_STACK_OVERFLOW:
    GCLAssertMsg(false, "One of the internal stacks was already filled, and the user tried to add on to the full stack.");
    break;
  case IL_STACK_UNDERFLOW:
    GCLAssertMsg(false, "One of the internal stacks was empty, and the user tried to empty the already empty stack.");
    break;
  case IL_INVALID_CONVERSION:
    GCLAssertMsg(false, "An invalid conversion attempt was tried.");
    break;
  case IL_LIB_JPEG_ERROR:
    GCLAssertMsg(false, "An error occurred in the libjpeg library.");
    break;
  case IL_LIB_PNG_ERROR:
    GCLAssertMsg(false, "An error occurred in the libpng library.");
    break;
  case IL_UNKNOWN_ERROR:
    GCLAssertMsg(false, "No function sets this yet, but it is possible (not probable) it may be used in the future.");
    break;
  case ILUT_NOT_SUPPORTED:
    GCLAssertMsg(false, "A type is valid but not supported in the current build.");
    break;
  default:
    GCLAssertMsg(false, "Don't know what happened.");
    break;

  }
  std::cerr << "il error: " << ret << std::endl;
}

inline void ilVersionCheck()
{
  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
    iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
    ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
      GCLAssertMsg(false, "DevIL version is different...exiting!\n");
  }
}
#endif
using namespace GCL;


Texture::Texture(const char *filename)
: mTextureId(-1)
{
  bool ret = LoadTexture(filename);
  GCLAssertMsg(ret, (std::string("Failed Loading Testure: ") + std::string(filename)).c_str());

  const TextureResource::TextureData &tempTextureData = mTextureResource->mTextureData;
  Initialize(tempTextureData.mWidth, tempTextureData.mHeight, 4, tempTextureData.imageData);
}

Texture::Texture(size_t width, size_t height, size_t bytesPerPixel )
{
  Initialize(width, height, bytesPerPixel);
}

void Texture::Initialize(size_t width, size_t height, size_t bypesPerPixel, const uint8_t *data )
{
  glGenTextures(1, &mTextureId); glErrorCheck();
   glBindTexture(GL_TEXTURE_2D, mTextureId);glErrorCheck();
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glErrorCheck();
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);glErrorCheck();
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);glErrorCheck();
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);glErrorCheck();
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmapglErrorCheck();

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, data);glErrorCheck();
   glBindTexture(GL_TEXTURE_2D, 0);glErrorCheck();
}

bool Texture::LoadTexture(const char *filename)
{
  const Resource *tempResource = TextureResourceManager::Instance().LoadResource(filename);
  mTextureResource = static_cast<const TextureResource*>(tempResource);

  return mTextureResource != 0;
}
