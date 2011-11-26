#include "applayer/GCLRenderObject.h"
#include "applayer/GCLApplication.h"

using namespace GCL;

const   VertexPNT GCLRenderObject::square[4] = {
		{WorldPoint3(-0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)} ,
		{WorldPoint3(0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
		{WorldPoint3(0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
		{WorldPoint3(-0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } };
const VertexData GCLRenderObject::data = {&square, 4, VertexPNT::GetComponentType()};


GCLRenderObject::GCLRenderObject()
: RenderObject(Matrix44::IDENTITY)
{
	GCLApplication::RegisterRenderObject(this);
}

GCLRenderObject::~GCLRenderObject()
{
	GCLApplication::ReleaseRenderObject(this);
}
