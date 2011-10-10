#pragma once
#include "Renderer/OpenGL.h"


namespace GCL
{
  class TextureResource;
  class Texture
  {
  public:
    Texture(const char *filename);
    Texture(size_t width, size_t height, size_t bypesPerPixel = 4);
    ~Texture()
    {
      glDeleteTextures(1, &mTextureId); glErrorCheck();
    }
    bool LoadTexture(const char *filename);

    void Bind() { GCLAssert(IsValid()); glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();}

    bool IsValid() const { return (int)mTextureId!=-1; }

    void Save(const char *filename) { GCLAssert(false); }

    static void ResetDefault() { GCLAssert(false); }

  private:
    void Initialize(size_t width, size_t height, size_t bypesPerPixel, const uint8_t *imageData = NULL );
    friend class FrameBuffer;
    GLuint GetTextureId() const { return mTextureId; }
    GLuint mTextureId;

    const TextureResource *mTextureResource;
  };

}
