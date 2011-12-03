#pragma once

#include <renderer/Material.h>
#include <renderer/RenderObject.h>
#include <renderer/Vertex.h>

namespace GCL
{
class GCLRenderObject : public RenderObject
{
public:
	GCLRenderObject();

	~GCLRenderObject();
	const VertexData &GetVertexData() const
	{
		return data;
	}

	 const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;
	//friend class GCLApplication;
	static const VertexPNT square[4];
	static const VertexData data;


};

}
