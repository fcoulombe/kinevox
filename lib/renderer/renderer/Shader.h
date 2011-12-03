#pragma once
#include "renderer/OpenGL.h"

namespace GCL
{
class Matrix44;
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

    void SetProjectionMatrix(const Matrix44 &m);
    void SetModelViewMatrix(const Matrix44 &m);
  private:
    void PrintInfoLog(GLuint );
    GLuint CompileShader(const char *shaderSrc, GLenum type);

    GLuint mProgramObject;
    bool mIsValid;
  };
}
