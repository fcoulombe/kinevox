
#include "renderer/GeomUtil.h"
#include <gcl/Math.h>

using namespace GCL;

void GeomUtil::MakeMeshSphere(std::vector<WorldPoint3>& dst, WorldUnit radius)
{

	const size_t resolution = 16;
	const WorldUnit halfPI = PI * 0.5;
	WorldUnit interp = 1.0 / (WorldUnit)resolution;
	WorldPoint3 v0,v1,v2,v3;
	radius *= 0.5;

	for(size_t i=0; i<=resolution; ++i)
	{
		WorldUnit theta0 = interp*(WorldUnit)(i+0)*PI - halfPI;
		WorldUnit theta1 = interp*(WorldUnit)(i+1)*PI - halfPI;
		WorldUnit z1 = std::sin(theta0);
		WorldUnit z2 = std::sin(theta1);

		v0.z = z1 * radius;
		v1.z = z1 * radius;
		v2.z = z2 * radius;
		v3.z = z2 * radius;

		for(size_t j=0; j<=resolution; ++j)
		{
			WorldUnit phi0 = interp*(WorldUnit)(j+0)*2.0*PI;
			WorldUnit phi1 = interp*(WorldUnit)(j+1)*2.0*PI;
			WorldUnit x1 = std::cos(theta0)*std::cos(phi0);
			WorldUnit x2 = std::cos(theta0)*std::cos(phi1);
			WorldUnit y1 = std::cos(theta0)*std::sin(phi0);
			WorldUnit y2 = std::cos(theta0)*std::sin(phi1);
			WorldUnit x3 = std::cos(theta1)*std::cos(phi0);
			WorldUnit x4 = std::cos(theta1)*std::cos(phi1);
			WorldUnit y3 = std::cos(theta1)*std::sin(phi0);
			WorldUnit y4 = std::cos(theta1)*std::sin(phi1);
			v0.x = x1 * radius;
			v0.y = y1 * radius;
			v1.x = x2 * radius;
			v1.y = y2 * radius;
			v2.x = x3 * radius;
			v2.y = y3 * radius;
			v3.x = x4 * radius;
			v3.y = y4 * radius;

			dst.push_back(v0); dst.push_back(v1); dst.push_back(v2);
			dst.push_back(v2); dst.push_back(v1); dst.push_back(v3);
		}
	}
}

void GeomUtil::MakeMeshCircle(std::vector<WorldPoint3>& dst, WorldUnit /*radius*/)
{
	const size_t resolution = 64;
	WorldUnit interp = 1.0 / (WorldUnit)resolution;
	WorldPoint3 middle(0.0, 0.0, 0.0);
	WorldPoint3 v0(0.0, 0.0, 0.0);
	WorldPoint3 v1(0.0, 0.0, 0.0);

	for(size_t i = 0; i<=resolution; ++i)
	{
		WorldUnit alpha0 = (WorldUnit)(i+0) * interp * PI * 2.0;
		WorldUnit alpha1 = (WorldUnit)(i+1) * interp * PI * 2.0;

		v0.x = sin(alpha0);
		v0.y = cos(alpha0);
		v1.x = sin(alpha1);
		v1.y = cos(alpha1);

		dst.push_back(v0);
		dst.push_back(middle);
		dst.push_back(v1);
	}
}

void GeomUtil::MakeMeshPlane(std::vector<WorldPoint3>& vertexData,
		std::vector<WorldPoint3>& tcoordData,
		WorldUnit size)
{
	const WorldPoint3 v0( 1.0 * size,  1.0 * size,  0.0);
	const WorldPoint3 v1(-1.0 * size,  1.0 * size,  0.0);
	const WorldPoint3 v2( 1.0 * size, -1.0 * size,  0.0);
	const WorldPoint3 v3(-1.0 * size, -1.0 * size,  0.0);

	const WorldPoint3 t0( 1.0,  1.0,  0.0);
	const WorldPoint3 t1( 0.0,  1.0,  0.0);
	const WorldPoint3 t2( 1.0,  0.0,  0.0);
	const WorldPoint3 t3( 0.0,  0.0,  0.0);


	vertexData.push_back(v0);
	vertexData.push_back(v1);
	vertexData.push_back(v2);
	vertexData.push_back(v2);
	vertexData.push_back(v1);
	vertexData.push_back(v3);

	tcoordData.push_back(t0);
	tcoordData.push_back(t1);
	tcoordData.push_back(t2);
	tcoordData.push_back(t2);
	tcoordData.push_back(t1);
	tcoordData.push_back(t3);
}

void GeomUtil::MakeMeshCube(std::vector<WorldPoint3>& vertexData,
		std::vector<WorldPoint3>& tcoordData,
		WorldUnit size)
{
	size *= 0.5;

	const WorldPoint3 v0( 1.0 * size,  1.0 * size, -1.0 * size);
	const WorldPoint3 v1(-1.0 * size,  1.0 * size, -1.0 * size);
	const WorldPoint3 v2( 1.0 * size,  1.0 * size,  1.0 * size);
	const WorldPoint3 v3(-1.0 * size,  1.0 * size,  1.0 * size);
	const WorldPoint3 v4( 1.0 * size, -1.0 * size, -1.0 * size);
	const WorldPoint3 v5(-1.0 * size, -1.0 * size, -1.0 * size);
	const WorldPoint3 v6( 1.0 * size, -1.0 * size,  1.0 * size);
	const WorldPoint3 v7(-1.0 * size, -1.0 * size,  1.0 * size);

	const WorldPoint3 t0( 1.0,  1.0,  0.0);
	const WorldPoint3 t1( 0.0,  1.0,  0.0);
	const WorldPoint3 t2( 1.0,  0.0,  0.0);
	const WorldPoint3 t3( 0.0,  0.0,  0.0);

	/* Top */
	vertexData.push_back(v0);
	vertexData.push_back(v1);
	vertexData.push_back(v2);
	vertexData.push_back(v2);
	vertexData.push_back(v1);
	vertexData.push_back(v3);
	/* Bottom */
	vertexData.push_back(v5);
	vertexData.push_back(v4);
	vertexData.push_back(v7);
	vertexData.push_back(v7);
	vertexData.push_back(v4);
	vertexData.push_back(v6);

	/* Right */
	vertexData.push_back(v0);
	vertexData.push_back(v2);
	vertexData.push_back(v4);
	vertexData.push_back(v4);
	vertexData.push_back(v2);
	vertexData.push_back(v6);

	/* Left */
	vertexData.push_back(v3);
	vertexData.push_back(v1);
	vertexData.push_back(v7);
	vertexData.push_back(v7);
	vertexData.push_back(v1);
	vertexData.push_back(v5);
	/* Back */
	vertexData.push_back(v0);
	vertexData.push_back(v1);
	vertexData.push_back(v4);
	vertexData.push_back(v4);
	vertexData.push_back(v1);
	vertexData.push_back(v5);
	/* Front */
	vertexData.push_back(v2);
	vertexData.push_back(v3);
	vertexData.push_back(v6);
	vertexData.push_back(v6);
	vertexData.push_back(v3);
	vertexData.push_back(v7);

	for(size_t i=0; i<6; ++i)
	{
		tcoordData.push_back(t0);
		tcoordData.push_back(t1);
		tcoordData.push_back(t2);
		tcoordData.push_back(t2);
		tcoordData.push_back(t1);
		tcoordData.push_back(t3);
	}
}
