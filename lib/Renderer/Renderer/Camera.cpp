#include <iostream>
#include "Renderer/Camera.h"

#include <GCL/Math.h>
#include "Renderer/OpenGL.h"

using namespace GCL;

Camera::Camera()
{
	mMatrix = Matrix44::IDENTITY;
	mMatrix[3].z = -10;
}

void Camera::Update()
{
	glLoadMatrixd(reinterpret_cast<const GLdouble*>(&mMatrix));
}


void Camera::MoveForward()
{
	std::cout << "back" << std::endl;
	mMatrix[3] += mMatrix[2]/10;

}
void Camera::MoveBackward()
{
	std::cout << "forward" << std::endl;
	mMatrix[3] -= mMatrix[2]/10;
}
void Camera::TurnLeft()
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(1));
	mMatrix = rotMat*mMatrix;
}
void Camera::TurnRight()
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(-1));
	mMatrix = rotMat*mMatrix;
}
void Camera::TiltUp()
{

}
void Camera::TiltDown()
{

}

