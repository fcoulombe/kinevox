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

    void Bind() const { GCLAssert(IsValid()); glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();}

    bool IsValid() const { return (int)mTextureId!=-1; }

    void Save(const char *filename);

    static void ResetDefault() { GCLAssert(false); }

  private:
    void Initialize(size_t width, size_t height, size_t bypesPerPixel, const uint8_t *imageData = NULL );
    const uint8_t *GetTextureFromVRAM() const;

    struct TextureData
    {
      size_t width, height;
      size_t GetImageSizeInBytes() const { return width*height*3; }
    }mTextureData;

    friend class FrameBuffer;
    GLuint GetTextureId() const { return mTextureId; }
    GLuint mTextureId;

    const TextureResource *mTextureResource;
  };

}
