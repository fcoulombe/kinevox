#pragma once

#include <stdint.h>
#include <gcl/Matrix44.h>
#include "renderer/Vertex.h"


namespace GCL
{
class Material;
struct VertexData
{
	const void *mVertexData;
	size_t vertexCount;
	uint32_t vertexType;
};
GCLINLINE std::ostream& operator<<( std::ostream& output, const VertexData &P)
{
	const uint8_t *currentVertex = reinterpret_cast<const uint8_t *>(P.mVertexData);
	for (size_t i=0; i<P.vertexCount; ++i)
	{
		size_t offset=0;
		if (P.vertexType & ePOSITION)
		{
			output << "Position: " << *reinterpret_cast<const WorldPoint3*>(currentVertex);
			offset+= sizeof(WorldPoint3);
		}
		if (P.vertexType & eNORMAL)
		{
			output << " Normal: " << *reinterpret_cast<const WorldPoint3*>(currentVertex+offset);
			offset+= sizeof(WorldPoint3);
		}
		if (P.vertexType & eTEXTURE_COORD)
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
	RenderObject(const Matrix44 &transform)
	: mTransform(transform)
	{
	}
	virtual ~RenderObject() {}
	virtual const VertexData &GetVertexData() const=0;
	virtual const Material &GetMaterial() const=0;
	const Matrix44 &GetTransform() const {return mTransform; }

	void SetOrientation(WorldUnit x,WorldUnit y,WorldUnit z)
	{
		mTransform.SetRotationX(x);
		mTransform.SetRotationY(y);
		mTransform.SetRotationZ(z);
	}
	void SetPosition(WorldUnit x, WorldUnit y,WorldUnit z)
	{ SetPosition(WorldPoint3(x,y,z));	}
	void SetPosition(const WorldPoint3 &position)
	{
		mTransform.SetPosition(position);
	}

protected:
	Matrix44 mTransform;
};
}
