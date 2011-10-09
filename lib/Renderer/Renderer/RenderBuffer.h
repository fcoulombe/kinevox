#pragma once
#include "Renderer/OpenGL.h"


namespace GCL
{
  class RenderBuffer
  {
  public:
    RenderBuffer()
    : mRenderBufferId(-1)
    {
      glGenRenderbuffers(1, &mRenderBufferId); glErrorCheck();
    }
    ~RenderBuffer()
    {
      glDeleteRenderbuffers(1, &mRenderBufferId); glErrorCheck();
    }
    void Bind() { GCLAssert(IsValid()); glBindRenderbuffer(GL_RENDERBUFFER_EXT, mRenderBufferId); glErrorCheck(); }

    bool IsValid() const { return (int)mRenderBufferId!=-1; }

    void Save(const char *filename) { GCLAssert(false); }

    static void ResetDefault() { GCLAssert(false); }

  private:
    GLuint mRenderBufferId;
  };

}
