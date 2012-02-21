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
#include <gcl/Point3.h>
#include "renderer/OpenGL.h"

namespace GCL
{

inline void DrawCube(const WorldPoint3 &position, Real size)
{
	const Real halfSize = size/2;
#if !defined(ES1) && !defined(ES2)
	glBegin(GL_QUADS);
	//front face
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z+halfSize);
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z+halfSize);

	//backface
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z-halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z-halfSize);
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z-halfSize);
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z-halfSize);

	//bot face
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z-halfSize);
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z-halfSize);

	//top face
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z-halfSize);
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z-halfSize);

	//left face
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z+halfSize);
	glVertex3d(position.x-halfSize, position.y+halfSize, position.z-halfSize);
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z-halfSize);
	glVertex3d(position.x-halfSize, position.y-halfSize, position.z+halfSize);

	//right face
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z+halfSize);
	glVertex3d(position.x+halfSize, position.y+halfSize, position.z-halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z-halfSize);
	glVertex3d(position.x+halfSize, position.y-halfSize, position.z+halfSize);
	glEnd();
#else
    
    GCLAssertMsg(false, "TBD");
    
#endif
	glErrorCheck();

}

inline void WriteCube(WorldPoint3* cubeData, const WorldPoint3 &position, Real size)
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
