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
#elif OS_WIN32
#   include <windows.h>
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glext.h>
// function pointers for PBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#else
#   error "TBD"
#endif

#if USE_64BIT_PLATFORM
#	define GL_UNIT GL_DOUBLE
#	define GLreal GLdouble
#	define glLoadMatrix glLoadMatrixd
#	define glMultMatrix glMultMatrixd
#else
#	define GL_UNIT GL_FLOAT
#	define GLreal GLfloat
#	define glLoadMatrix glLoadMatrixf
#	define glMultMatrix glMultMatrixf
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

#if defined(ES1 ) || 1
#define ENABLE_SHADERS 0
#else
#define ENABLE_SHADERS 1
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
