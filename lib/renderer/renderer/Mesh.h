#pragma once


#include "renderer/Material.h"
#include "renderer/VertexBuffer.h"

namespace GCL
{

  class Mesh
  {
  public:
	  Mesh(const char *filename="DefaultMesh");
	  void Render();
  private:
	  Material mMaterial;
	  VertexBuffer<VertexPNT> *mVertexBuffer;
  };

}
