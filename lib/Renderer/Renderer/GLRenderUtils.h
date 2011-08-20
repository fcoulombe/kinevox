#pragma once

#include <iostream>
#include <GCL/Point3.h>
#include "Renderer/OpenGL.h"

namespace GCL
{

inline void DrawCube(const WorldPoint3 &position, double size)
{
	double halfSize = size/2;

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

}
}
