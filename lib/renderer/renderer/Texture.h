#pragma once
#include "renderer/OpenGL.h"


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

    void Bind() const
    {
    	//std::cout << "Binding Texture: " <<mTextureId << std::endl;
    	GCLAssert(IsValid());
    	glBindTexture(GL_TEXTURE_2D, mTextureId);  glErrorCheck();
    }

    bool IsValid() const { return (int)mTextureId!=-1; }

    void Save(const char *filename);

    static void ResetDefault() { GCLAssert(false); }

  private:
    void Initialize(size_t width, size_t height, size_t bypesPerPixel, const uint8_t *imageData = NULL );
    const uint8_t *GetTextureFromVRAM() const;

    struct TextureData
    {
      size_t width, height, bytesPerPixel;
      size_t GetImageSizeInBytes() const { return width*height*bytesPerPixel; }
    }mTextureData;

    friend class FrameBuffer;
    GLuint GetTextureId() const { return mTextureId; }
    GLuint mTextureId;

    const TextureResource *mTextureResource;
  };

}
