#include <iostream>
#include "renderer/Camera.h"

#include <gcl/Assert.h>
#include <gcl/Math.h>
#include "renderer/OpenGL.h"

using namespace GCL;


Camera &Camera::DefaultCamera()
{
	static Camera defaultCamera;
	return defaultCamera; }

Camera::Camera()
: mMatrix(Matrix44::IDENTITY)
{
	//TurnLeft(180);
	//mMatrix[3].z = -10;
}

void Camera::Update() const
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
	WorldPoint4 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = rotMat*mMatrix;
	mMatrix[3] = positionBack;
}
void Camera::TurnRight(double degree )
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(degree));
	WorldPoint4 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = rotMat*mMatrix;
	mMatrix[3] = positionBack;
}
void Camera::TiltUp()
{
	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(1));
	WorldPoint4 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = mMatrix*rotMat;
	mMatrix[3] = positionBack;
}
void Camera::TiltDown()
{

	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(-1));
	WorldPoint4 positionBack = mMatrix[3];
	mMatrix[3] = WorldPoint3::ZERO;
	mMatrix = mMatrix*rotMat;
	mMatrix[3] = positionBack;
}

