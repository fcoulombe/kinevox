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
#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/Vertex.h"


namespace GCL
{

template<typename VertexType>
class GLESVertexBuffer
{
public:
	GLESVertexBuffer(const VertexType *vertexArray, size_t count)
	: mBufferType(GL_STATIC_DRAW),
	  mVertexCount(count)
	{
		glGenBuffers(1, &mVertexBufferId);glErrorCheck();
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);glErrorCheck();
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(VertexType)*count), (const GLvoid*)vertexArray, mBufferType);glErrorCheck();
	}


	~GLESVertexBuffer()
	{
		glDeleteBuffers(1, &mVertexBufferId);glErrorCheck();
	}
	void PreRender()
	{
#if ENABLE_FIX_PIPELINE
		if (VertexType::GetComponentType() & ePOSITION)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_UNIT, sizeof(VertexType), (char*)NULL+VertexType::OffsetToPosition()); glErrorCheck();
		}
		if (VertexType::GetComponentType() & eNORMAL)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_UNIT, sizeof(VertexType), (char*)NULL+VertexType::OffsetToNormal()); glErrorCheck();
		}
		if (VertexType::GetComponentType() & eTEXTURE_COORD)
		{
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_UNIT, sizeof(VertexType), (char*)NULL+VertexType::OffsetToTextureCoordinate());glErrorCheck();
		}
#else
		if (VertexType::GetComponentType() & ePOSITION)
		{
            glEnableVertexAttribArray(ATTRIB_POSITION);glErrorCheck();
			glVertexAttribPointer(ATTRIB_POSITION, 3, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToPosition());glErrorCheck();
		}
		if (VertexType::GetComponentType() & eNORMAL)
		{
            glEnableVertexAttribArray(ATTRIB_NORMAL);glErrorCheck();
            glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_UNIT, GL_TRUE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToNormal());glErrorCheck();
		}
		if (VertexType::GetComponentType() & eTEXTURE_COORD)
		{
			glEnableVertexAttribArray(ATTRIB_TEXTURE_COORD);glErrorCheck();
			glVertexAttribPointer(ATTRIB_TEXTURE_COORD, 2, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToTextureCoordinate());glErrorCheck();
		}
#endif
	}

	void Render(int mode = GL_TRIANGLES)
	{
		PreRender();
		glDrawArrays((GLenum)mode, 0, (GLsizei)mVertexCount);glErrorCheck();
		PostRender();
	}

	void PostRender()
	{
#if ENABLE_FIX_PIPELINE
		if (VertexType::GetComponentType() & ePOSITION)
		{
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		if (VertexType::GetComponentType() & eNORMAL)
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if (VertexType::GetComponentType() & eTEXTURE_COORD)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

#else
		if (VertexType::GetComponentType() & ePOSITION)
		{
			glDisableVertexAttribArray(ATTRIB_POSITION);glErrorCheck();
		}
		if (VertexType::GetComponentType() & eNORMAL)
		{
			glDisableVertexAttribArray(ATTRIB_NORMAL);glErrorCheck();
		}
		if (VertexType::GetComponentType() & eTEXTURE_COORD)
		{
			glDisableVertexAttribArray(ATTRIB_TEXTURE_COORD);glErrorCheck();
		}
#endif
	}

	bool IsValid() const { return mVertexBufferId!=(GLuint)-1; }

private:
	GLuint mVertexBufferId;
	GLuint mBufferType;
	size_t mVertexCount;
};

}
