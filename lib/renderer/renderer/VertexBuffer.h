#pragma once
#include "renderer/OpenGL.h"
#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/Vertex.h"


namespace GCL
{

template<typename VertexType>
class VertexBuffer
{
public:
	VertexBuffer(const VertexType *vertexArray, size_t count)
	: mBufferType(GL_STATIC_DRAW),
	  mVertexCount(count)
	{
		glGenBuffers(1, &mVertexBufferId);glErrorCheck();
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);glErrorCheck();
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType)*count, (void*)vertexArray, mBufferType);glErrorCheck();
	}


	~VertexBuffer()
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
			glVertexAttribPointer(ATTRIB_POSITION, 3, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToPosition());glErrorCheck();
			glEnableVertexAttribArray(ATTRIB_POSITION);glErrorCheck();
		}
		if (VertexType::GetComponentType() & eNORMAL)
		{
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_UNIT, GL_TRUE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToNormal());glErrorCheck();
			glEnableVertexAttribArray(ATTRIB_NORMAL);glErrorCheck();
		}
		if (VertexType::GetComponentType() & eTEXTURE_COORD)
		{
			glVertexAttribPointer(ATTRIB_TEXTURE_COORD, 2, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToTextureCoordinate());glErrorCheck();
			glEnableVertexAttribArray(ATTRIB_TEXTURE_COORD);glErrorCheck();
		}
#endif
	}

	void Render()
	{
		PreRender();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexCount);glErrorCheck();
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
