#pragma once
#include "renderer/OpenGL.h"



namespace GCL
{
class Texture;
class RenderBuffer;
  class FrameBuffer
  {
  public:
    FrameBuffer(const Texture &texture, const RenderBuffer &depthBuffer);
    ~FrameBuffer()
    {
      glDeleteFramebuffers(1, &mFrameBufferId); glErrorCheck();
    }
    void Bind() 
      { 
          GCLAssert(IsValid()); glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);  glErrorCheck();
      }

    bool IsValid() const { return (int)mFrameBufferId!=-1; }

    void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }

    static void ResetDefault() 
      {  
          glBindFramebuffer(GL_FRAMEBUFFER, 0);  glErrorCheck(); 
      }

  private:
    GLuint mFrameBufferId;
  };

}
