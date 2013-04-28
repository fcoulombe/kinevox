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

#define VERTEXBUFFER_INCLUDE GET_GFX_INCLUDE(VertexBuffer.h)
#define IVertexBuffer GET_GFX_CLASS(VertexBuffer)
#include VERTEXBUFFER_INCLUDE


namespace GCL
{

template<typename VertexType>
class VertexBuffer
{
public:
	VertexBuffer(const VertexType *vertexArray, size_t count)
		: mPimpl(vertexArray, count)
	{
	}


	~VertexBuffer()
	{

	}
	void PreRender()
	{
		mPimpl.PreRender();
	}

	void Render()
	{
		mPimpl.Render();
	}
	void Render(int mode)
	{
		mPimpl.Render(mode);
	}

	void PostRender()
	{
		mPimpl.PostRender();
	}

	bool IsValid() const { return mPimpl.IsValid(); }

private:
	IVertexBuffer<VertexType> mPimpl;
};

}
