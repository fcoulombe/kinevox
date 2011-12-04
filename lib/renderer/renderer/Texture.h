#pragma once
#include <gcl/Macro.h>
#include "renderer/OpenGL.h"


namespace GCL
{
  class TextureResource;

  struct TextureData
  {
    size_t width, height, bytesPerPixel;
    size_t GetImageSizeInBytes() const { return width*height*bytesPerPixel; }
  };

  class Texture
  {
  public:
    Texture(const char *filename);
    Texture(size_t width, size_t height, size_t bypesPerPixel = 4);
    ~Texture()
    {

    	glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
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


    friend class Shader;
    friend class FrameBuffer;
    GLuint GetTextureId() const { return mTextureId; }
    GLuint mTextureId;
    TextureData mTextureData;

    const TextureResource *mTextureResource;
  };

	GCLINLINE std::ostream& operator<<( std::ostream& output, const TextureData &P)
	{
		output << "(width: " << P.width << ", height: " << P.height << ", bpp: " << P.bytesPerPixel << ")" << std::endl;
		return output;
	}

}
