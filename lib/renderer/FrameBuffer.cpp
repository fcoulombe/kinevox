#include "renderer/FrameBuffer.h"

#include <sstream>

#include "renderer/RenderBuffer.h"
#include "renderer/Texture.h"

using namespace GCL;

namespace
{
void checkFrameBufferStatus(GLenum stat)
{
	if (GL_FRAMEBUFFER_COMPLETE == stat)
		return;
#if defined(ES1) || defined(ES2)
	GCLAssert(GL_FRAMEBUFFER_COMPLETE == stat);
#else
	std::stringstream s;
	switch (stat)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		s << "GL_FRAMEBUFFER_UNDEFINED is returned if target is the default framebuffer, but the default framebuffer does not exist.";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
		s << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete.";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		s<< "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT is returned if the framebuffer does not have at least one image attached to it.";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		s << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAWBUFFERi.";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		s << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.";
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		s << "GL_FRAMEBUFFER_UNSUPPORTED is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		s << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.";
		s << std::endl;
		s << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.";
		break;
		/* case  GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		  s<< "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.";*/
	default:
		s<<"don't know what happenned to the framebuffer";
	}
    
	GCLAssertMsg(GL_FRAMEBUFFER_COMPLETE == stat, s.str().c_str());
#endif

}
}

FrameBuffer::FrameBuffer(const Texture & texture, const RenderBuffer & depthBuffer)
: mFrameBufferId(-1)
{
	GCLAssert(texture.IsValid());

	glGenFramebuffers(1, &mFrameBufferId); glErrorCheck();
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId); glErrorCheck();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureId(), 0); glErrorCheck();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.GetRenderBufferId());glErrorCheck();

	GLenum status =  glCheckFramebufferStatus(GL_FRAMEBUFFER);
	checkFrameBufferStatus(status);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);glErrorCheck();
}
