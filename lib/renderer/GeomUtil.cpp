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

#include "renderer/GeomUtil.h"
#include <gcl/Math.h>

using namespace GCL;

void GeomUtil::MakeMeshSphere(std::vector<WorldPoint3>& dst, Real radius)
{
	const size_t resolution = 16;
	const Real halfPI = PI * 0.5;
	Real interp = 1.0 / (Real)resolution;
	WorldPoint3 v0,v1,v2,v3;
	radius *= 0.5;

	for(size_t i=0; i<=resolution; ++i)
	{
		Real theta0 = interp*(Real)(i+0)*PI - halfPI;
		Real theta1 = interp*(Real)(i+1)*PI - halfPI;
		Real z1 = std::sin(theta0);
		Real z2 = std::sin(theta1);

		v0.z = z1 * radius;
		v1.z = z1 * radius;
		v2.z = z2 * radius;
		v3.z = z2 * radius;

		for(size_t j=0; j<=resolution; ++j)
		{
			Real phi0 = interp*(Real)(j+0)*2.0*PI;
			Real phi1 = interp*(Real)(j+1)*2.0*PI;
			Real x1 = std::cos(theta0)*std::cos(phi0);
			Real x2 = std::cos(theta0)*std::cos(phi1);
			Real y1 = std::cos(theta0)*std::sin(phi0);
			Real y2 = std::cos(theta0)*std::sin(phi1);
			Real x3 = std::cos(theta1)*std::cos(phi0);
			Real x4 = std::cos(theta1)*std::cos(phi1);
			Real y3 = std::cos(theta1)*std::sin(phi0);
			Real y4 = std::cos(theta1)*std::sin(phi1);
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

void GeomUtil::MakeMeshCircle(std::vector<WorldPoint3>& dst, Real /*radius*/)
{
	const size_t resolution = 64;
	Real interp = 1.0 / (Real)resolution;
	WorldPoint3 middle(0.0, 0.0, 0.0);
	WorldPoint3 v0(0.0, 0.0, 0.0);
	WorldPoint3 v1(0.0, 0.0, 0.0);

	for(size_t i = 0; i<=resolution; ++i)
	{
		Real alpha0 = (Real)(i+0) * interp * PI * 2.0;
		Real alpha1 = (Real)(i+1) * interp * PI * 2.0;

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
		Real size)
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
		Real size)
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
