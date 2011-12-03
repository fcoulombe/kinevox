#pragma once

#include "renderer/OpenGL.h"
#include "renderer/Resource.h"
namespace GCL
{
  //this is a resource class that can be shared between difference texture instance
  class TextureResource : public Resource
  {
    public:

    TextureResource(const char *textureName);
    ~TextureResource();

    struct TextureData
    {
      TextureData()
      : mTextureHandle((GLuint)-1),
          mMinFilter(GL_LINEAR),
          mMagFilter(GL_LINEAR),
          //mWrapModeS(GL_CLAMP),
          //mWrapModeT(GL_CLAMP)
          mWrapModeS(GL_CLAMP_TO_EDGE),
          mWrapModeT(GL_CLAMP_TO_EDGE)
      {
        imageData = NULL;
      }
      ~TextureData()
      {

      }

      GLubyte *imageData;
      GLuint mTextureHandle;
      GLenum mMinFilter;
      GLenum mMagFilter;
      GLenum mWrapModeS;
      GLenum mWrapModeT;
      GLenum mTextureFormat;
      GLuint mBytePerPixel;
      GLuint mBitdepth;
      GLsizei mWidth,
              mHeight;
    }mTextureData;

    static const TextureResource EmptyTexture;

    static void LoadPng(FILE *is, TextureData &data);
    static void LoadTga(std::istream &is, TextureData &data);
    static void LoadRaw(std::istream &is, TextureData &data);
    static void Unload(TextureData &data);
  private:
    TextureResource() {}
  };
}
