#pragma once
#include "renderer/OpenGL.h"

namespace GCL
{
class Matrix44;
class Texture;
  class Shader
  {
  public:
    Shader();

    ~Shader()
    {
      if (mIsValid)
        glDeleteProgram(mProgramObject);glErrorCheck();
    }
    void Bind();
    bool IsValid() const { return mIsValid; }

    void SetTextureSampler(const Texture &sampler);
    void SetProjectionMatrix(const Matrix44 &m);
    void SetModelViewMatrix(const Matrix44 &m);
    void GetUniform(const char *unformName, Matrix44 &m44) const;
    void GetUniform(const char *unformName, int &ret) const;
    int GetAttributeLocation(const char *attributeName) const;

    static void ResetDefault();
  private:
    void PrintInfoLog(GLuint );
    GLuint CompileShader(const char *shaderSrc, GLenum type);

    GLuint mProgramObject;
    bool mIsValid;
  };
}
