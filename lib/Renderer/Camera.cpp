#include <iostream>
#include "Renderer/Camera.h"

#include <GCL/Math.h>
#include "Renderer/OpenGL.h"

using namespace GCL;

Camera::Camera()
{
	mMatrix = Matrix44::IDENTITY;
	TurnLeft(180);
	//mMatrix[3].z = -10;
}

void Camera::Update()
{
	Matrix44 tempMatrix = Inverse(mMatrix);
	glLoadMatrixd(reinterpret_cast<const GLdouble*>(&tempMatrix));glErrorCheck();
}


void Camera::MoveForward()
{
	mMatrix[3] -= mMatrix[2]/10;
}
void Camera::MoveBackward()
{
	mMatrix[3] += mMatrix[2]/10;
}
void Camera::TurnLeft(double degree )
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(degree));
	WorldPoint3 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = rotMat*mMatrix;
	mMatrix[3] = positionBack;
}
void Camera::TurnRight(double degree )
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(degree));
	WorldPoint3 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = rotMat*mMatrix;
	mMatrix[3] = positionBack;
}
void Camera::TiltUp()
{
	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(1));
	WorldPoint3 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = mMatrix*rotMat;
	mMatrix[3] = positionBack;
}
void Camera::TiltDown()
{

	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(-1));
	WorldPoint3 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = mMatrix*rotMat;
	mMatrix[3] = positionBack;
}

