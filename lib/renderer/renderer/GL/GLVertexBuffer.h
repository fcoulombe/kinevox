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
#include <3rdparty/OpenGL.h>
#include "renderer/Vertex.h"
#include "renderer/GL/GLVertexArrayObject.h"


namespace GCL
{
	const GLint GLVertexBufferMode[] = 
	{
		GL_TRIANGLES,
		GL_LINES,
		GL_TRIANGLE_STRIP
	};
class GLVertexBuffer
{
public:
	template<typename VertexType>
	GLVertexBuffer(const VertexType *vertexArray, size_t count, const AttribLocations &loc)
	: mBufferType(GL_STATIC_DRAW),
	  mVertexCount(count),
	  mVao(vertexArray)
	{
		RenderPipe::SendCommand([this, vertexArray, count, loc](){
		glGenBuffers(1, &mVertexBufferId);glErrorCheck();
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);glErrorCheck();
		mVao.PostInitUnsafe(vertexArray, loc);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType)*count, (void*)vertexArray, mBufferType);glErrorCheck();
		});
	}

	~GLVertexBuffer()
	{
		RenderPipe::SendCommandSync([&](){
		glDeleteBuffers(1, &mVertexBufferId);glErrorCheck();
		});
	}

	void Render(int mode = GL_TRIANGLES)
	{
		RenderPipe::SendCommand([this, mode](){
		mVao.BindUnsafe();
		glDrawArrays((GLenum)mode, 0, (GLsizei)mVertexCount);glErrorCheck();
		mVao.UnBindUnsafe();
		});
	}

	bool IsValid() const 
	{ 
		bool ret;
		RenderPipe::SendCommandSync([&](){ret = IsValidUnsafe();});
		return ret;
	}
	bool IsValidUnsafe() const { return mVertexBufferId!=(GLuint)-1; }

	static GLint GetRenderMode(size_t mode)
	{
		GCLAssert(mode<sizeof(GLVertexBufferMode)/sizeof(GLint));
		return GLVertexBufferMode[mode];
	}
private:
	GLuint mVertexBufferId;
	GLuint mBufferType;
	size_t mVertexCount;
	GLVertexArrayObject mVao;
};
}
