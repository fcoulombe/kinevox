/*
 * Copyright (C) 2013 by Francois Coulombe
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

#include <renderer/GeomUtil.h>
#include <renderer/Vertex.h>
#include <renderer/RenderObject.h>


namespace GCL
{
static const   VertexPNT square[6] = {
        {Point3<MeshReal>(-0.5, -0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0) ,Point2<MeshReal>(0.0, 0.0)},
        {Point3<MeshReal>(0.5, -0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(1.0, 0.0)},
        {Point3<MeshReal>(-0.5, 0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 1.0)},
        {Point3<MeshReal>(-0.5, 0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 1.0)},
        {Point3<MeshReal>(0.5, -0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(1.0, 0.0)},
        {Point3<MeshReal>(0.5, 0.5, 0.0), 	Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(1.0, 1.0)}
    };
class RenderObjectHelper
{
public:
	virtual ~RenderObjectHelper()
	{
		delete obj;
	}
	RenderObject *GetRenderObject() { return obj; }
	void SetTransform(const Matrix44 &transform) {obj->SetTransform(transform); }

	void SetOrientation(Real x,Real y,Real z) {	obj->SetOrientation(x,y,z);	}
	void SetPosition(Real x, Real y,Real z)
	{ obj->SetPosition(x,y,z);	}
	void SetPosition(const WorldPoint3 &position)  { obj->SetPosition(position); }
	const Matrix44 &GetTransform() const {return obj->GetTransform(); }
	Material &GetMaterial() const { return obj->GetMaterial(); }
	void SetEnableDrawNormals(bool enableDrawNormals=true) { obj->SetEnableDrawNormals(enableDrawNormals); }
protected:
	RenderObject *obj;
	Material material;
};
class SquareRenderObject : public RenderObjectHelper
{
public:
	SquareRenderObject( const Matrix44 &transform = Matrix44(true))
	{
		obj = new RenderObject( transform, material, square, 6);
    }
};


class SphereRenderObject : public RenderObjectHelper
{
public:
    SphereRenderObject(const Matrix44 &transform= Matrix44(true))
    {
        std::vector<WorldPoint3 > v;
        GeomUtil::MakeMeshSphere(v, 1.0);
		std::vector<Point3<MeshReal> > vertex;
        for (size_t i=0; i<v.size(); ++i)
        {
            vertex.push_back(Point3<MeshReal>(v[0]));
        }
		obj = new RenderObject(transform, material, (const VertexP*)vertex.data(), vertex.size());
    }
};


class CircleRenderObject : public RenderObjectHelper
{
public:
    CircleRenderObject(const Matrix44 &transform= Matrix44(true))
    {
        std::vector<WorldPoint3 > v;
        GeomUtil::MakeMeshCircle(v, 1.0);
        std::vector<Point3<MeshReal> > vertex;
		for (size_t i=0; i<v.size(); ++i)
        {
            vertex.push_back(Point3<MeshReal>(v[0]));
        }

		obj = new RenderObject( transform, material,(const VertexP*)vertex.data(), vertex.size() );
    }
};

class PlaneRenderObject : public RenderObjectHelper
{
public:
    PlaneRenderObject(const Matrix44 &transform= Matrix44(true))
    {
        std::vector<WorldPoint3> points;
        std::vector<WorldPoint2> uvs;
        GeomUtil::MakeMeshPlane(points, uvs, 1.0);
		std::vector<VertexPT> vertex;
        for (size_t i=0; i<points.size(); ++i)
        {
            VertexPT v; 
            v.position = points[i];
            v.textureCoordinate = uvs[i];
            vertex.push_back(v);
        }
		obj = new RenderObject( transform, material, (const VertexPT*)vertex.data(), vertex.size() );
    }

};

class CubeRenderObject : public RenderObjectHelper
{
public:
    CubeRenderObject( const Matrix44 &transform= Matrix44(true))
    {
        std::vector<WorldPoint3> points;
        std::vector<WorldPoint3> normals;
        std::vector<WorldPoint2> uvs;
        GeomUtil::MakeMeshCube(points, normals, uvs, 1.0);
        std::vector<VertexPNT> vertex;
        for (size_t i=0; i<points.size(); ++i)
        {
            VertexPNT v; 
            v.position = points[i];
            v.normal = normals[i];
            v.textureCoordinate = uvs[i];
            vertex.push_back(v);
        }
        VertexPNT *positions =  (VertexPNT *)(vertex.data());
		obj = new RenderObject( transform, material, positions, vertex.size() );
    }

};
}