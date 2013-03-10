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
#include <renderer/RenderObject.h>
#include <renderer/Vertex.h>

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
class SquareRenderObject : public RenderObject
{
public:
	SquareRenderObject(const char *name = "SquareRenderObject", const Matrix44 &transform = Matrix44(true))
	: RenderObject(name, transform) //identity
	{
        data.push_back(VertexData(&square, 6, VertexPNT::GetComponentType()));
    }
	const VertexDataList &GetVertexData() const
	{
		return data;
	}
private:
	VertexDataList data;

};


class SphereRenderObject : public RenderObject
{
public:
    SphereRenderObject(const char *name="SphereRenderObject", const Matrix44 &transform= Matrix44(true))
        : RenderObject(name, transform) //identity
    {
        std::vector<WorldPoint3 > v;
        GeomUtil::MakeMeshSphere(v, 1.0);
        for (size_t i=0; i<v.size(); ++i)
        {
            vertex.push_back(Point3<MeshReal>(v[0]));
        }
       
        data.push_back(VertexData(vertex.data(), vertex.size(), VertexP::GetComponentType()));
    }
    const VertexDataList &GetVertexData() const
    {
        return data;
    }
private:
    std::vector<Point3<MeshReal> > vertex;
    VertexDataList data;
};


class CircleRenderObject : public RenderObject
{
public:
    CircleRenderObject(const char *name="CircleRenderObject", const Matrix44 &transform= Matrix44(true))
        : RenderObject(name, transform) //identity
    {
        std::vector<WorldPoint3 > v;
        GeomUtil::MakeMeshCircle(v, 1.0);
        for (size_t i=0; i<v.size(); ++i)
        {
            vertex.push_back(Point3<MeshReal>(v[0]));
        }
        data.push_back(VertexData(vertex.data(), vertex.size(), VertexP::GetComponentType()));
    }
    const VertexDataList &GetVertexData() const
    {
        return data;
    }
private:
    std::vector<Point3<MeshReal> > vertex;
    VertexDataList data;
};

class PlaneRenderObject : public RenderObject
{
public:
    PlaneRenderObject(const char *name="PlaneRenderObject", const Matrix44 &transform= Matrix44(true))
        : RenderObject(name, transform) //identity
    {
        std::vector<WorldPoint3> points;
        std::vector<WorldPoint2> uvs;
        GeomUtil::MakeMeshPlane(points, uvs, 1.0);
        for (size_t i=0; i<points.size(); ++i)
        {
            VertexPT v; 
            v.position = points[i];
            v.textureCoordinate = uvs[i];
            vertex.push_back(v);
        }
        data.push_back(VertexData(vertex.data(), points.size(), VertexPT::GetComponentType()));
    }
    const VertexDataList &GetVertexData() const
    {
        return data;
    }
private:
    std::vector<VertexPT> vertex;
    VertexDataList data;
};

class CubeRenderObject : public RenderObject
{
public:
    CubeRenderObject(const char *name = "CubeRenderObject", const Matrix44 &transform= Matrix44(true))
        : RenderObject(name, transform) //identity
    {
        std::vector<WorldPoint3> points;
        std::vector<WorldPoint3> normals;
        std::vector<WorldPoint2> uvs;
        GeomUtil::MakeMeshCube(points, normals, uvs, 1.0);
        
        for (size_t i=0; i<points.size(); ++i)
        {
            VertexPNT v; 
            v.position = points[i];
            v.normal = normals[i];
            v.textureCoordinate = uvs[i];
            vertex.push_back(v);
        }
        VertexPNT *positions =  (VertexPNT *)(vertex.data());
        data.push_back(VertexData(positions, vertex.size(), VertexPNT::GetComponentType()));
    }
    const VertexDataList &GetVertexData() const
    {
        return data;
    }
private:
    std::vector<VertexPNT> vertex;
    VertexDataList data;
};
}