/*
 * Copyright (C) 2013 by Francois Coulombe
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
#include <string>
#include <vector>
#include <gcl/Matrix44.h>

namespace GCL
{
enum RenderCommandType
{
	SWAP_BUFFER,
	CREATE_RENDERER,
	CREATE_VERTEX_BUFFER,
	CREATE_TEXTURE,
	CREATE_SHADER,
	CREATE_VAO,
	GET_VENDOR,
	GET_VERSION,
	GET_RENDERER,
	GET_SHADING_LANGUAGE_VERSION,
	GET_GLEW_VERSION,
	GET_EXTENSIONS,
	GET_IS_EXTENSION_SUPPORTED,
	GET_IS_GLEW_EXTENSION_SUPPORTED,
	GET_GL_PROJECTION,
	GET_GL_MODELVIEW,
	SET_VIEWPORT,
	SET_PROJECTION,
	SET_MODELVIEW,
	RenderCommandMax
};
class RenderCommand
{
public:
	RenderCommand(RenderCommandType cmd, void *data=NULL)
	{
		mCmd = cmd;
		mData = data;
	}
	//virtual void Run(RenderData &renderThread)=0;
	RenderCommandType mCmd;
	void *mData;
};
struct RenderData
{
	size_t pad;
};
typedef void (*RenderCommandFunction)(void *, RenderData &);

class ReturnMessage
{
public:
	enum ReturnMessageType
	{
		RMT_BOOL,
		RMT_NUMBER,
		RMT_FLOAT,
		RMT_STRING,
		RMT_MATRIX4,
		RMT_STRINGLIST
	};
	ReturnMessage(bool data)
	{
		mType = RMT_BOOL;
		mData = (data) ? (void*)1 : (void*)0;
	}
	ReturnMessage(const std::string &data)
	{
		mType = RMT_STRING;
		mData = new std::string(data);
	}
	ReturnMessage(const std::vector<std::string> &data)
	{
		mType = RMT_STRINGLIST;
		mData = new std::vector<std::string>(data);
	}
	ReturnMessage(const Matrix44 &data)
	{
		mType = RMT_MATRIX4;
		mData = new Matrix44(data);
	}
	~ReturnMessage();
	
	const std::string &GetString() const
	{
		return *(std::string*)(mData);
	}
	const Matrix44 &GetMatrix() const
	{
		return *(Matrix44*)(mData);
	}
	const bool GetBool() const
	{
		return (mData) ? true : false;
	}
	const std::vector<std::string> &GetStringList() const
	{
		return *(std::vector<std::string>*)(mData);
	}
private:
	ReturnMessageType mType;
	void *mData;
};


}
