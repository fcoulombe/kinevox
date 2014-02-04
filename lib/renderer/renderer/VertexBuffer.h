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
#include "renderer/ShaderAttributeLocations.h"
#include "renderer/Vertex.h"
#include "rendererconf.h"
#include GFXAPI_VertexBuffer_H


namespace GCL
{
	enum VertexBufferMode
	{
		VBM_TRIANGLES=0,
		VBM_LINES=1,
		VBM_TRIANGLE_STRIP=2
	};
	class VertexBuffer
	{
	public:
		template<typename VertexType>
		VertexBuffer(const VertexType *vertexArray, size_t count, const AttribLocations &loc)
		{
			mVertexBuffer = new IVertexBuffer(vertexArray, count, loc);
		}
		~VertexBuffer()
		{
			delete mVertexBuffer;
		}

		void Render(VertexBufferMode mode=VBM_TRIANGLES) const
		{
			mVertexBuffer->Render(IVertexBuffer::GetRenderMode(mode));
		}

		bool IsValid() const { return mVertexBuffer->IsValid(); }
	private:
		IVertexBuffer *mVertexBuffer;
	};
}
