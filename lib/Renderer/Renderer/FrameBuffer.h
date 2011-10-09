#pragma once
#include "Renderer/OpenGL.h"


namespace GCL
{
  class FrameBuffer
  {
  public:
    FrameBuffer()
    : mFrameBufferId(-1)
    {
      glGenFramebuffers(1, &mFrameBufferId); glErrorCheck();
    }
    ~FrameBuffer()
    {
      glDeleteFramebuffers(1, &mFrameBufferId); glErrorCheck();
    }
    void Bind() { GCLAssert(IsValid()); glBindFramebuffer(GL_FRAMEBUFFER_EXT, mFrameBufferId);  glErrorCheck();}

    bool IsValid() const { return (int)mFrameBufferId!=-1; }

    void Save(const char *filename) { GCLAssert(false); }

    static void ResetDefault() { GCLAssert(false); }

  private:
    GLuint mFrameBufferId;
  };

}
