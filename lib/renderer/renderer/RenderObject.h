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

#include <stdint.h>
#include <gcl/Matrix44.h>
#include "renderer/Vertex.h"


namespace GCL
{
class Material;
struct VertexData
{
	VertexData()
	{
		mVertexData = NULL;
		mVertexCount = 0;
		mVertexType = 0;
	}
	VertexData(	const void *vertexData,size_t vertexCount,	uint32_t vertexType)
	{
		mVertexData = vertexData;
		mVertexCount = vertexCount;
		mVertexType = vertexType;
	}

	const void *mVertexData;
	size_t mVertexCount;
	uint32_t mVertexType;
};
GCLINLINE std::ostream& operator<<( std::ostream& output, const VertexData &P)
{
	const uint8_t *currentVertex = reinterpret_cast<const uint8_t *>(P.mVertexData);
	for (size_t i=0; i<P.mVertexCount; ++i)
	{
		size_t offset=0;
		if (P.mVertexType & ePOSITION)
		{
			output << "Position: " << *reinterpret_cast<const WorldPoint3*>(currentVertex);
			offset+= sizeof(WorldPoint3);
		}
		if (P.mVertexType & eNORMAL)
		{
			output << " Normal: " << *reinterpret_cast<const WorldPoint3*>(currentVertex+offset);
			offset+= sizeof(WorldPoint3);
		}
		if (P.mVertexType & eTEXTURE_COORD)
		{
			output << " TexCoord: " << *reinterpret_cast<const WorldPoint2*>(currentVertex+offset)<<std::endl;
			offset+= sizeof(WorldPoint2);
		}
		currentVertex+= offset;
	}
	return output;
}

class RenderObject
{
public:
	RenderObject(const char *name, const Matrix44 &transform)
	: mObjName(name)
	{
		mTransform = (transform);
	}
	virtual ~RenderObject() {}
	virtual const VertexData &GetVertexData() const=0;
	virtual const Material &GetMaterial() const=0;
	const Matrix44 &GetTransform() const {return mTransform; }
	void SetTransform(const Matrix44 &transform) {mTransform = transform; }

	void SetOrientation(Real x,Real y,Real z)
	{

		const WorldPoint4 backupPosition = mTransform[3];
		Matrix44 xRot;
		xRot.SetRotationX(x);
		Matrix44 yRot;
		yRot.SetRotationY(y);
		Matrix44 zRot;
		zRot.SetRotationZ(z);
		mTransform = xRot * yRot;// * zRot;

		mTransform.SetPosition(backupPosition);
	}
	void SetPosition(Real x, Real y,Real z)
	{ SetPosition(WorldPoint3(x,y,z));	}
	void SetPosition(const WorldPoint3 &position)
	{
		mTransform.SetPosition(position);
	}

protected:
	Matrix44 mTransform;
	std::string mObjName;
};
}
