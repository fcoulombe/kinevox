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



#if defined(OS_IPHONE)
#elif defined(OS_MACOSX)
#elif OS_LINUX
#   include <AL/al.h>
#   include <AL/alc.h>
#elif OS_WIN32
#   include <al.h>
#   include <alc.h>
#else
#   error "TBD"
#endif


#include <gcl/Assert.h>
#include <sstream>
#define alErrorCheck() do { i_alErrorCheck(__FILE__, __LINE__); }while(0);
inline void i_alErrorCheck(const char *file, int line)
{
	ALenum err = alGetError();
	if (err == AL_NO_ERROR)
		return;

	std::stringstream s;
	s<<file << ":" <<  line << ": OpenAL Error" << std::endl;
	switch (err)
	{
	case AL_INVALID_NAME:
		s<<"a bad name (ID) was passed to an OpenAL function";
		break;
	case AL_INVALID_ENUM:
	  s<<"an invalid enum value was passed to an OpenAL function";
		break;
	case AL_INVALID_VALUE:
	  s<<"an invalid value was passed to an OpenAL function";
		break;
	case AL_INVALID_OPERATION:
	 s<<"the requested operation is not valid";
		break;
	case AL_OUT_OF_MEMORY:
	  s<<"the requested operation resulted in OpenAL running out of memory";
		break;
	default:
	  s<<"OpenAL: don't know what happenedl";
	}
	GCLAssertMsg(false, s.str().c_str());

}
