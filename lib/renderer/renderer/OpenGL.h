#pragma once


#if ENABLE_GLEW
#include <GL/glew.h>
#endif

#if defined(OS_IPHONE)
#   if defined( ES2)
#       include <OpenGLES/ES2/gl.h>
#       include <OpenGLES/ES2/glext.h>
#   elif defined(ES1)
#       include <OpenGLES/ES1/gl.h>
#       include <OpenGLES/ES1/glext.h>
#       define ENABLE_FIX_PIPELINE 1
#   endif
#elif defined(OS_MACOSX) 
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <OpenGL/glext.h>
#elif OS_LINUX
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glext.h>
#else
#   error "TBD"
#endif

#if USE_64BIT_PLATFORM
#define GL_UNIT GL_DOUBLE
#define GLreal GLdouble
#define glLoadMatrix glLoadMatrixd
#define glMultMatrix glMultMatrixd
#else
#define GL_UNIT GL_FLOAT
#define GLreal GLfloat
#define glLoadMatrix glLoadMatrixf
#define glMultMatrix glMultMatrixf
#endif

#ifdef ES1
#define glClearDepth                            glClearDepthf
#define glGenerateMipmap                        glGenerateMipmapOES
#define glGenFramebuffers                       glGenFramebuffersOES
#define glGenRenderbuffers                       glGenRenderbuffersOES
#define glBindRenderbuffer                       glBindRenderbufferOES
#define glBindFramebuffer                       glBindFramebufferOES
#define glFramebufferTexture2D          glFramebufferTexture2DOES
#define glDeleteFramebuffers            glDeleteFramebuffersOES
#define glDeleteRenderbuffers            glDeleteRenderbuffersOES
#define glCheckFramebufferStatus        glCheckFramebufferStatusOES
#define glDeleteVertexArrays            glDeleteVertexArraysOES
#define glGenVertexArrays                       glGenVertexArraysOES
#define glBindVertexArray                       glBindVertexArrayOES
#define glRenderbufferStorage glRenderbufferStorageOES
#define glFramebufferRenderbuffer glFramebufferRenderbufferOES

#define GL_RENDERBUFFER                       GL_RENDERBUFFER_OES
#define GL_FRAMEBUFFER                       GL_FRAMEBUFFER_OES
#define GL_FRAMEBUFFER_BINDING       GL_FRAMEBUFFER_BINDING_OES
#define GL_COLOR_ATTACHMENT0         GL_COLOR_ATTACHMENT0_OES
#define GL_FRAMEBUFFER_COMPLETE      GL_FRAMEBUFFER_COMPLETE_OES
#define GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_OES

#endif


#include <gcl/Assert.h>
#include <sstream>
#define glErrorCheck() do { i_glErrorCheck(__FILE__, __LINE__); }while(0);
inline void i_glErrorCheck(const char *file, int line)
{
	GLenum err = glGetError();
	if (err == GL_NO_ERROR)
		return;

	std::stringstream s;
	s<<file << ":" <<  line << ": OpenGL Error" << std::endl;
	switch (err)
	{
	case GL_INVALID_ENUM:
	  s<<"An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_VALUE:
	  s<<"A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_OPERATION:
	 s<<"The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
#ifndef ES2
	case GL_STACK_OVERFLOW:
	  s<<"This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_STACK_UNDERFLOW:
	  s<<"This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
#endif
#if !defined(ES2) && !defined(ES1)
        case GL_TABLE_TOO_LARGE:
            s<< "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
            break;
#endif
	case GL_OUT_OF_MEMORY:
	  s<<"There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		break;
#ifndef ES1
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        s<<"The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than to set the error flag.";
            break;

#endif
	default:
	  s<<"OpenGL: don't know what happenedl";
	}
	GCLAssertMsg(false, s.str().c_str());

}
