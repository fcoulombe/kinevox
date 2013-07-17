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

#pragma once

#include <iostream>
#include <gcl/Assert.h>
#include <renderer/VertexBuffer.h>

namespace GCL
{

GCLINLINE void DrawCube(const WorldPoint3 &position, Real size)
{
	const Real halfSize = size/2;
    VertexP posData[24];
	//front face
    posData[0].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));
	posData[1].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));
	posData[2].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize), MeshReal(position.z+halfSize));
	posData[3].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z+halfSize));

	//backface
	posData[4].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));
	posData[5].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));
	posData[6].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));
	posData[7].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));

	//bot face
	posData[8].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));
	posData[9].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));
	posData[10].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));
	posData[11].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));

	//top face
	posData[12].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z+halfSize));
	posData[13].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize),MeshReal( position.z+halfSize));
	posData[14].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));
	posData[15].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));

	//left face
	posData[16].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z+halfSize));
	posData[17].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));
	posData[18].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));
	posData[19].position = Point3<MeshReal>(MeshReal(position.x-halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));

	//right face
	posData[20].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize), MeshReal(position.z+halfSize));
	posData[21].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y+halfSize), MeshReal(position.z-halfSize));
	posData[22].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z-halfSize));
	posData[23].position = Point3<MeshReal>(MeshReal(position.x+halfSize), MeshReal(position.y-halfSize), MeshReal(position.z+halfSize));
    //VertexBuffer<VertexP> buffer(posData, 24);
    //buffer.PreRender();
    //buffer.Render();
    //buffer.PostRender();
}

GCLINLINE void WriteCube(WorldPoint3* cubeData, const WorldPoint3 &position, Real size)
{
	const Real halfSize = size/2;

	//front face
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z+halfSize); ++cubeData;

	//backface
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z-halfSize); ++cubeData;

	//bot face
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z-halfSize); ++cubeData;

	//top face
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z-halfSize); ++cubeData;

	//left face
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z+halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y+halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z-halfSize); ++cubeData;
	*cubeData = WorldPoint3(position.x-halfSize, position.y-halfSize, position.z+halfSize); ++cubeData;

	//right face
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z+halfSize);
	*cubeData = WorldPoint3(position.x+halfSize, position.y+halfSize, position.z-halfSize);
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z-halfSize);
	*cubeData = WorldPoint3(position.x+halfSize, position.y-halfSize, position.z+halfSize);
}
}
