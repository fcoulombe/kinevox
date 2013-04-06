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
#include <3rdparty/OpenRL.h>
#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/Vertex.h"


namespace GCL
{
    struct AttribLocations
    {
        RLint position;
        RLint texCoord;
        RLint normal;
    };
template<typename VertexType>
class RLVertexBuffer
{
public:
	RLVertexBuffer(const VertexType *vertexArray, size_t count)
	: mBufferType(RL_STATIC_DRAW),
	  mVertexCount(count)
	{
        rlGenPrimitives(1, &mPrimitive);
        rlBindPrimitive(RL_PRIMITIVE, mPrimitive);

		rlGenBuffers(1, &mVertexBufferId);glErrorCheck();
		rlBindBuffer(RL_ARRAY_BUFFER, mVertexBufferId);glErrorCheck();
		rlBufferData(RL_ARRAY_BUFFER, sizeof(VertexType)*count, (void*)vertexArray, mBufferType);glErrorCheck();
	}
    void BindShaderLoc(const AttribLocations &loc)
    {
        if (VertexType::GetComponentType() & ePOSITION)
        {
            rlVertexAttribBuffer(loc.position, 3, RL_UNIT, RL_FALSE, sizeof(VertexType), VertexType::OffsetToPosition());
        }
        if (VertexType::GetComponentType() & eNORMAL)
        {
            rlVertexAttribBuffer(loc.normal, 3, RL_UNIT, RL_TRUE, sizeof(VertexType), VertexType::OffsetToNormal());
        }
        if (VertexType::GetComponentType() & eTEXTURE_COORD)
        {
            rlVertexAttribBuffer(loc.texCoord, 2, RL_UNIT, RL_FALSE, sizeof(VertexType), VertexType::OffsetToTextureCoordinate());
        }
    }


	~RLVertexBuffer()
	{
		rlDeleteBuffers(1, &mVertexBufferId);glErrorCheck();
	}


	void Render(RLenum mode = RL_TRIANGLES)
	{
		rlDrawArrays(mode, 0, (RLint)mVertexCount);glErrorCheck();
	}

	bool IsValid() const { return mVertexBufferId!=(RLbuffer)-1; }

private:
    RLprimitive mPrimitive;
	RLbuffer  mVertexBufferId;
	RLenum mBufferType;
	size_t mVertexCount;
};

}
