#pragma once

#include <vector>
#include <gcl/Point3.h>
namespace GCL
{
  class GeomUtil
  {
  public:
	  static void MakeMeshSphere(std::vector<WorldPoint3>& dst, Real radius);
	  static void MakeMeshCircle(std::vector<WorldPoint3>& dst, Real radius);
	  static void MakeMeshPlane(std::vector<WorldPoint3>& vertexData,
	  		  std::vector<WorldPoint3>& tcoordData,
	  		  Real size);
	  static void MakeMeshCube(std::vector<WorldPoint3>& vertexData,
	  		  std::vector<WorldPoint3>& tcoordData,
	  		  Real size);

  };
}

