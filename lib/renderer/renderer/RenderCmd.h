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
	SWAP_BUFFER,//0,
	CREATE_RENDERER,//1,
	DESTROY_RENDERER,//2,
	RENDERER_PRERENDER,
	RENDERER_POSTRENDER,
	RENDERER_RENDER,
	RENDERER_GET_VENDOR,//3,
	RENDERER_GET_VERSION,//4,
	RENDERER_GET_RENDERER,//5,
	RENDERER_GET_SHADING_LANGUAGE_VERSION,//6,
	RENDERER_GET_GLEW_VERSION,//7,
	RENDERER_GET_EXTENSIONS,//8,
	RENDERER_GET_IS_EXTENSION_SUPPORTED,//9,
	RENDERER_GET_IS_GLEW_EXTENSION_SUPPORTED,//10,
	RENDERER_GET_GL_PROJECTION,//11,
	RENDERER_GET_GL_MODELVIEW,//12,
	RENDERER_GET_MODELVIEW,//12,
	RENDERER_GET_PROJECTION,//11,
	RENDERER_SET_VIEWPORT,//13,
	RENDERER_SET_CAMERA,//14,
	RENDERER_SET_ORTHO,//15,
	GPUPROGRAM_CREATE,//16,
	GPUPROGRAM_DESTROY,//17,
	GPUPROGRAM_BIND,//18,
	GPUPROGRAM_ATTACH_SHADER,//19,
	GPUPROGRAM_LINK,//20,
	IS_GPUPROGRAM_VALID,//21,
	GPUPROGRAM_SET_TEXTURE_SAMPLER,//22,
	GPUPROGRAM_SET_PROJECTION,//23,
	GPUPROGRAM_SET_MODELVIEW,//24,
	GPUPROGRAM_SET_UNIFORM_NUMBER,
	GPUPROGRAM_SET_UNIFORM_VEC2i,
	GPUPROGRAM_SET_UNIFORM_VEC2f,
	GPUPROGRAM_GET_UNIFORM_MATRIX,//25,
	GPUPROGRAM_GET_UNIFORM_NUMBER,//26,
	GPUPROGRAM_GET_ATTRIBUTE_LOCATION,//27,
	GPUPROGRAM_RESETDEFAULT,//28,
	SHADER_CREATE,//29,
	SHADER_DESTROY,//30,
	IS_SHADER_VALID,//31,
	TEXTURE_DESTROY,//32,
	TEXTURE_CREATE,//33,
	TEXTURE_BIND,//34,
	IS_TEXTURE_VALID,//35,
	TEXTURE_GET_WIDTH,//36,
	TEXTURE_GET_HEIGHT,//37,
	TEXTURE_GET_BPP,//38,
	TEXTURE_GET_TEXTURE_FROM_VRAM,//39,
	TEXTURE_GET_PIXELBUFFER_FROM_VRAM,//40,
	FRAMEBUFFER_CREATE,//41,
	FRAMEBUFFER_DESTROY,//42,
	FRAMEBUFFER_BIND,//43,
	IS_FRAMEBUFFER_VALID,//44,
	FRAMEBUFFER_RESETDEFAULT,//45,
	RENDERBUFFER_CREATE,//46,
	RENDERBUFFER_DESTROY,//47,
	RENDERBUFFER_BIND,//48,
	IS_RENDERBUFFER_VALID,//49,
	VBO_CREATE,//50,
	VBO_DESTROY,//51,
	VBO_RENDER,//52,
	IS_VBO_VALID,//53,
	PBO_CREATE,//54,
	PBO_DESTROY,//55,
	PBO_UNBIND,//56,
	PBO_BIND,//57,
	PBO_PUSH,//58,
	PBO_PULL,//59,
	IS_PBO_VALID,//60,
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
	RenderCommandType mCmd;
	void *mData;
};
class RenderCommandMatArg : public RenderCommand
{
public:
	RenderCommandMatArg(RenderCommandType cmd, void *data, const Matrix44 &data2)
		:RenderCommand(cmd, data)
	{
		mData2 = data2;
	}
	Matrix44 mData2;
};

class RenderCommand2Arg : public RenderCommand
{
public:
	RenderCommand2Arg(RenderCommandType cmd, void *data, void *data2)
		:RenderCommand(cmd, data)
	{
		mData2 = data2;
	}
	void *mData2;
};

class RenderCommandVec2fArg : public RenderCommand2Arg
{
public:
	RenderCommandVec2fArg(RenderCommandType cmd, void *data, void *data2, const Point2<float> &data3)
		:RenderCommand2Arg(cmd, data, data2)
	{
		mData3 = data3;
	}
	Point2<float> mData3;
};
class RenderCommandVec2iArg : public RenderCommand2Arg
{
public:
	RenderCommandVec2iArg(RenderCommandType cmd, void *data, void *data2, const Point2<int> &data3)
		:RenderCommand2Arg(cmd, data, data2)
	{
		mData3 = data3;
	}
	Point2<int> mData3;
};

class RenderCommand3Arg : public RenderCommand2Arg
{
public:
	RenderCommand3Arg(RenderCommandType cmd, void *data, void *data2, void *data3)
		:RenderCommand2Arg(cmd, data, data2)
	{
		mData3 = data3;
	}
	void *mData3;
};
class RenderCommand4Arg : public RenderCommand3Arg
{
public:
	RenderCommand4Arg(RenderCommandType cmd, void *data, void *data2, void *data3, void *data4)
		:RenderCommand3Arg(cmd, data, data2, data3)
	{
		mData4 = data4;
	}
	void *mData4;
};
class RenderCommand5Arg : public RenderCommand4Arg
{
public:
	RenderCommand5Arg(RenderCommandType cmd, void *data, void *data2, void *data3, void *data4,  void *data5)
		:RenderCommand4Arg(cmd, data, data2, data3, data4)
	{
		mData5 = data5;
	}
	void *mData5;
};
struct RenderData
{
	RenderData()
		: pad(0xf2f2f2f2)
	{
	}
	virtual ~RenderData(){}
	size_t pad;
};
typedef void (*RenderCommandFunction)(RenderCommand *, RenderData &);

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
		RMT_STRINGLIST,
		RMT_POINTER
	};
	ReturnMessage(bool data)
	{
		mType = RMT_BOOL;
		mData = (data) ? (void*)1 : (void*)0;
	}
	ReturnMessage(int data)
	{
		mType = RMT_NUMBER;
		mData = (void*)(data) ;
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
	ReturnMessage(const void *data)
	{
		mType = RMT_POINTER;
		mData = (void*)data;
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
	const int GetNumber() const
	{
		return (int)(mData);
	}
	const void* GetPointer() const
	{
		return (mData);
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
