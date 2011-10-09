#pragma once
#include "Renderer/OpenGL.h"

namespace GCL
{
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

  private:
    void PrintInfoLog(GLuint );
    GLuint CompileShader(const char *shaderSrc, GLenum type);

    GLuint mProgramObject;
    bool mIsValid;
  };
}
