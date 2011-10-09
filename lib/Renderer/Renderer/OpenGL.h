#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
/* If using GLEW */
//#include <GL/glew.h>
#include <GL/gl.h>
#endif


#include <GCL/Assert.h>
#include <iostream>
#define glErrorCheck() do { i_glErrorCheck(__FILE__, __LINE__); }while(0);
inline void i_glErrorCheck(const char *file, int line)
{
	GLenum err = glGetError();
	if (err == GL_NO_ERROR)
		return;

	std::cerr << file << ":" <<  line << ": OpenGL Error" << std::endl;
	switch (err)
	{
	case GL_INVALID_ENUM:
	  GCLAssert(false &&"An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_VALUE:
	  GCLAssert(false && "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_OPERATION:
	  GCLAssert(false && "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_STACK_OVERFLOW:
	  GCLAssert(false && "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_STACK_UNDERFLOW:
	  GCLAssert(false && "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_OUT_OF_MEMORY:
	  GCLAssert(false && "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
		break;
	case GL_TABLE_TOO_LARGE:
	  GCLAssert(false && "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	default:
	  GCLAssert(false && "OpenGL: don't know what happenedl");
	}

}
