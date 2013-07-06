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
#include <3rdparty/OpenGL.h>
#include "renderer/ShaderAttributeLocations.h"
#include "renderer/Vertex.h"


namespace GCL
{


class GLVertexArrayObject
{
public:
	template<typename VertexType>
	GLVertexArrayObject(const VertexType *) //argument is to make the template happy.
	{
		mType = VertexType::GetComponentType();
#if ENABLE_FIX_PIPELINE
		(void)loc;
		mVertexSize = sizeof(VertexType);
		
		if (mType & ePOSITION)
			mOffsetList.push_back(VertexType::OffsetToPosition());
		if (mType & eNORMAL)
			mOffsetList.push_back(VertexType::OffsetToNormal());
		if (mType & eTEXTURE_COORD)
			mOffsetList.push_back(VertexType::OffsetToTextureCoordinate());

#else
		glGenVertexArrays(1, &mVao); glErrorCheck();
		glBindVertexArray(mVao); glErrorCheck();

#endif
	}
	template<typename VertexType>
	void PostInit(const VertexType *, const AttribLocations &loc)
	{
		#if !ENABLE_FIX_PIPELINE

		if (mType & ePOSITION)
		{
			glEnableVertexAttribArray(loc.position);glErrorCheck();
			glVertexAttribPointer(loc.position, 3, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToPosition());glErrorCheck();
		}
		else
		{
			glDisableVertexAttribArray(loc.position);glErrorCheck();
		}
		if (mType & eNORMAL)
		{
			glEnableVertexAttribArray(loc.normal);glErrorCheck();
			glVertexAttribPointer(loc.normal, 3, GL_UNIT, GL_TRUE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToNormal());glErrorCheck();
		}
		else
		{
			glDisableVertexAttribArray(loc.normal);glErrorCheck();
		}
		if (mType & eTEXTURE_COORD)
		{
			glEnableVertexAttribArray(loc.texCoord);glErrorCheck();
			glVertexAttribPointer(loc.texCoord, 2, GL_UNIT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToTextureCoordinate());glErrorCheck();
		}
		else
		{
			glDisableVertexAttribArray(loc.texCoord);glErrorCheck();
		}
#endif
	}


	~GLVertexArrayObject()
	{
#if !ENABLE_FIX_PIPELINE
		glDeleteVertexArrays(1, &mVao);glErrorCheck();
#endif
	}
	void Bind()
	{
#if ENABLE_FIX_PIPELINE
		size_t i=0;
		if (mType & ePOSITION)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_UNIT, mVertexSize, (char*)NULL+mOffsetList[i++]); glErrorCheck();
		}
		if (mType & eNORMAL)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_UNIT, mVertexSize, (char*)NULL+mOffsetList[i++]); glErrorCheck();
		}
		if (mType & eTEXTURE_COORD)
		{
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_UNIT, mVertexSize, (char*)NULL+mOffsetList[i++]);glErrorCheck();
		}
#else
		 glBindVertexArray(mVao);glErrorCheck();
#endif
	}


	void UnBind()
	{
#if ENABLE_FIX_PIPELINE
		if (mType & ePOSITION)
		{
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		if (mType & eNORMAL)
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if (mType & eTEXTURE_COORD)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
#else
		 glBindVertexArray(0);glErrorCheck();
#endif
	}

	bool IsValid() const
	{
#if ENABLE_FIX_PIPELINE
		return true;
#else
		return (glIsVertexArray(mVao) == GL_TRUE);
#endif
		}

private:
	uint32_t mType;
	size_t mVertexSize;
#if ENABLE_FIX_PIPELINE
	std::vector<size_t> mOffsetList;
#else
	GLuint mVao;
#endif

};

}
