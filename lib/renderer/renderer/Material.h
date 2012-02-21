#pragma once

#include <gcl/Point4.h>
#include "renderer/ResourceManagerConfig.h"
namespace GCL
{

class Texture;
class Shader;
  class Material
  {
  public:
	Material(const char *filename="Default");

    ~Material()
    {

    }
    void Bind() const;
    void LoadMaterial(const char *filename);

    void SetTexture(Texture &texture);
  private:
    Texture *mTexture;
    Shader *mShader;

    WorldPoint4 mAmbient;
    WorldPoint4 mDiffuset;
    WorldPoint4 mspecular;
    WorldPoint4 mEmissive;
    Real mShininess;

  };

}
