#pragma once
#include "renderer/OpenGL.h"
#include "renderer/Texture.h"


namespace GCL
{
  class FrameBuffer
  {
  public:
    FrameBuffer(const Texture &texture, const RenderBuffer &depthBuffer)
    : mFrameBufferId(-1)
    {
      glGenFramebuffers(1, &mFrameBufferId); glErrorCheck();
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBufferId);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                GL_TEXTURE_2D, texture.GetTextureId(), 0);
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                   GL_RENDERBUFFER_EXT, depthBuffer.GetRenderBufferId());

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    }
    ~FrameBuffer()
    {
      glDeleteFramebuffers(1, &mFrameBufferId); glErrorCheck();
    }
    void Bind() { GCLAssert(IsValid()); glBindFramebuffer(GL_FRAMEBUFFER_EXT, mFrameBufferId);  glErrorCheck();}

    bool IsValid() const { return (int)mFrameBufferId!=-1; }

    void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }

    static void ResetDefault() {  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);  glErrorCheck(); }

  private:
    GLuint mFrameBufferId;
  };

}
