/*
 * Copyright (C) 2011 by Francois Coulombe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once


#if ENABLE_GLEW
#include <GL/glew.h>
#elif ENABLE_EGLEW
#include <eglew.h>
#endif
#if defined(ES1) || defined(ES2)
#define IS_PBO_SUPPORTED 0
#else
#define IS_PBO_SUPPORTED 1
#endif

#if defined(ES1)
#   include <EGL/egl.h>
#   include <GLES/gl.h>
//#   define GL_GLEXT_PROTOTYPES
#   include <GLES/glext.h>

#   define glGenerateMipmap         glGenerateMipmapOES

#   define glDeleteFramebuffers     glDeleteFramebuffersOES
#   define glBindFramebuffer        glBindFramebufferOES
#   define glGenFramebuffers        glGenFramebuffersOES
#   define glFramebufferTexture2D   glFramebufferTexture2DOES
#   define glFramebufferRenderbuffer glFramebufferRenderbufferOES
#   define glCheckFramebufferStatus glCheckFramebufferStatusOES

#   define glGenRenderbuffers       glGenRenderbuffersOES
#   define glBindRenderbuffer       glBindRenderbufferOES
#   define glRenderbufferStorage    glRenderbufferStorageOES
#   define glDeleteRenderbuffers    glDeleteRenderbuffersOES

#   define glClearDepth glClearDepthf

#   define GL_RENDERBUFFER          GL_RENDERBUFFER_OES
#   define GL_FRAMEBUFFER           GL_FRAMEBUFFER_OES
#   define GL_FRAMEBUFFER_BINDING   GL_FRAMEBUFFER_BINDING_OES
#   define GL_COLOR_ATTACHMENT0     GL_COLOR_ATTACHMENT0_OES
#   define GL_FRAMEBUFFER_COMPLETE  GL_FRAMEBUFFER_COMPLETE_OES
#   define GL_DEPTH_COMPONENT       GL_DEPTH_COMPONENT16_OES
#   define GL_DEPTH_ATTACHMENT      GL_DEPTH_ATTACHMENT_OES

#elif defined( ES2)
#   include <EGL/egl.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#define glGenBuffersARB glGenBuffers
#define glDeleteBuffersARB glDeleteBuffers

#define glBindBufferARB glBindBuffer
#define GL_STREAM_DRAW_ARB GL_STREAM_DRAW
#define glMapBufferARB glMapBuffer
#define glUnmapBufferARB glUnmapBuffer

#define glClearDepth                            glClearDepthf
#elif defined (ES3)
#define GL_PIXEL_UNPACK_BUFFER_ARB GL_PIXEL_UNPACK_BUFFER
#define GL_PIXEL_PACK_BUFFER_ARB GL_PIXEL_PACK_BUFFER
#elif defined(OS_MACOSX) 
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <OpenGL/glext.h>
#elif defined(OS_LINUX)
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glext.h>
#elif defined(OS_WIN32)
#   include <windows.h>
#   include <GL/gl.h>
#   include <GL/glu.h>
//#define GL_GLEXT_PROTOTYPES
#   include <GL/glext.h>
#   undef glGenFramebuffers 
#   define glGenFramebuffers glGenFramebuffersEXT
#   undef glBindFramebuffer 
#   define glBindFramebuffer glBindFramebufferEXT
#   undef glFramebufferTexture2D 
#   define glFramebufferTexture2D glFramebufferTexture2DEXT
#   undef glFramebufferRenderbuffer
#   define glFramebufferRenderbuffer glFramebufferRenderbufferEXT
#   undef glCheckFramebufferStatus 
#   define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#   undef glDeleteFramebuffers
#   define glDeleteFramebuffers glDeleteFramebuffersEXT
#   undef glGenRenderbuffers
#   define glGenRenderbuffers glGenRenderbuffersEXT
#   undef glBindRenderbuffer
#   define glBindRenderbuffer glBindRenderbufferEXT
#   undef glRenderbufferStorage
#   define glRenderbufferStorage glRenderbufferStorageEXT
#   undef glDeleteRenderbuffers
#   define glDeleteRenderbuffers glDeleteRenderbuffersEXT
#   undef glGenerateMipmap 
#   define glGenerateMipmap glGenerateMipmapEXT
#else
#   error "TBD"
#endif

#if USE_64BIT_PLATFORM && !defined(ES1) 
#	define GL_UNIT GL_FLOAT //our mesh data are passed in float for portability/performance reason
#	define GLreal GLdouble
#	define glLoadMatrix glLoadMatrixd
#	define glMultMatrix glMultMatrixd
#else
#	define GL_UNIT GL_FLOAT
#	define GLreal GLfloat
#	define glLoadMatrix glLoadMatrixf
#	define glMultMatrix glMultMatrixf
#endif

#if !defined(ES1 ) 
#define ENABLE_SHADERS 1
#else
#define ENABLE_SHADERS 0
#endif


static const GLenum BytePerPixel[] =
{
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA,
#if defined(ES1) || defined(ES2) || defined(ES3)
    GL_RGB,
    GL_RGBA
#else
    GL_RGB8,
    GL_RGBA8
#endif
};
static const GLenum BytesPerPixel[] =
{
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA,
    GL_RGB,
    GL_RGBA,
};
static const GLenum GLUInternalFormat[] =
{
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA,
    GL_RGB,
    GL_RGBA
};

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
		s<<"OpenGL: don't know what happened";
	}
	GCLAssertMsg(false, s.str().c_str());
}
