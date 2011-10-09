#pragma once
#include "Renderer/OpenGL.h"


namespace GCL
{
  class Texture
  {
  public:
    Texture(const char *filename)
    : mTextureId(-1)
    {
      bool ret = LoadTexture(filename);
      GCLAssert(ret);
      glGenTextures(1, &mTextureId); glErrorCheck();
      glBindTexture(GL_TEXTURE_2D, mTextureId);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, 0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    ~Texture()
    {
      glDeleteTextures(1, &mTextureId); glErrorCheck();
    }
    bool LoadTexture(const char *filename)
    {

    }
    void Bind() { GCLAssert(IsValid()); glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();}

    bool IsValid() const { return (int)mTextureId!=-1; }

    void Save(const char *filename) { GCLAssert(false); }

    static void ResetDefault() { GCLAssert(false); }

  private:
    GLuint mTextureId;
    size_t mWidth, mHeight;
  };

}
