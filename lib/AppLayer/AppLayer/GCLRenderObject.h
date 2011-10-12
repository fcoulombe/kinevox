#pragma once

#include <Renderer/RenderObject.h>
#include <Renderer/Vertex.h>

namespace GCL
{
class GCLRenderObject : public RenderObject
{
public:
	GCLRenderObject()
	: RenderObject(Matrix44::IDENTITY)
	{}

	const VertexData &GetVertexData() const
	{
		return data;
	}
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
private:

	static const VertexPNT square[4];
	static const VertexData data;


};

}
