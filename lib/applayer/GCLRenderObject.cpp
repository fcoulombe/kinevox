#include "applayer/GCLRenderObject.h"
#include "applayer/GCLApplication.h"

using namespace GCL;

const   VertexPNT GCLRenderObject::cube[GCLRenderObject::NUM_VERTICES] = {
		//front
		{WorldPoint3(-0.5, -0.5, 0.5), 	WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
		{WorldPoint3(0.5, -0.5, 0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
		{WorldPoint3(-0.5, 0.5, 0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(0.5, 0.5, 0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)},

		//back
		{WorldPoint3(-0.5, -0.5, -0.5), WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
		{WorldPoint3(0.5, -0.5, -0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
		{WorldPoint3(-0.5, 0.5, -0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(0.5, 0.5, -0.5), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)},

		//left
		{WorldPoint3(-0.5, -0.5, -0.5), WorldPoint3(-1.0, 0.0, 0.0) ,WorldPoint2(0.0, 0.0)},
		{WorldPoint3(-0.5, -0.5, 0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(1.0, 0.0)},
		{WorldPoint3(-0.5, 0.5, -0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(-0.5, 0.5, 0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(1.0, 1.0)},

		//right
		{WorldPoint3(0.5, -0.5, -0.5), WorldPoint3(-1.0, 0.0, 0.0) ,WorldPoint2(0.0, 0.0)},
		{WorldPoint3(0.5, -0.5, 0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(1.0, 0.0)},
		{WorldPoint3(0.5, 0.5, -0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(0.5, 0.5, 0.5), 	WorldPoint3(-1.0, 0.0, 0.0), WorldPoint2(1.0, 1.0)},

		//top
		{WorldPoint3(0.5, 0.5, 0.5), WorldPoint3(0.0, 1.0, 0.0) ,WorldPoint2(1.0, 0.0)},
		{WorldPoint3(0.5, 0.5, -0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(0.0, 0.0)},
		{WorldPoint3(-0.5, 0.5, -0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(-0.5, 0.5, 0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(1.0, 1.0)},

		//bottom
		{WorldPoint3(0.5, -0.5, 0.5), WorldPoint3(0.0, 1.0, 0.0) ,WorldPoint2(1.0, 0.0)},
		{WorldPoint3(0.5, -0.5, -0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(0.0, 0.0)},
		{WorldPoint3(-0.5,-0.5, -0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(-0.5, -0.5, 0.5), 	WorldPoint3(0.0, 1.0, 0.0), WorldPoint2(1.0, 1.0)}

};
const VertexData GCLRenderObject::data(&cube, GCLRenderObject::NUM_VERTICES, VertexPNT::GetComponentType());


GCLRenderObject::GCLRenderObject()
: RenderObject(Matrix44::IDENTITY),
  mMaterial("Default")
{
	GCLApplication::RegisterRenderObject(this);
}

GCLRenderObject::~GCLRenderObject()
{
	GCLApplication::ReleaseRenderObject(this);
}
