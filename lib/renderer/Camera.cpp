#include <iostream>
#include "renderer/Camera.h"

#include <gcl/Assert.h>
#include <gcl/Math.h>

#include "renderer/GLRenderer.h"
#include "renderer/OpenGL.h"

using namespace GCL;


Camera &Camera::DefaultCamera()
{
	static Camera defaultCamera;
	return defaultCamera;
}

Camera::Camera()
: mCameraMatrix(true), //identity
  mFov(45.0),
  mAspect(640.0/480.0),
  mNear(0.1),
  mFar(100.0)
{
}

void Camera::Update()
{
	mProjectionMatrix.SetPerspective(mFov,mAspect,mNear,mFar);
	mModelViewMatrix = Inverse(mCameraMatrix);
	//glLoadMatrixd(reinterpret_cast<const GLreal*>(&mModelViewMatrix));glErrorCheck();
}


void Camera::MoveForward()
{
	mCameraMatrix[3] -= mCameraMatrix[2]/10;
}
void Camera::MoveBackward()
{
	mCameraMatrix[3] += mCameraMatrix[2]/10;
}
void Camera::TurnLeft(Real degree )
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(degree));
	WorldPoint4 positionBack = mCameraMatrix[3];
	mCameraMatrix[3] = WorldPoint3::ZERO;
	mCameraMatrix = rotMat*mCameraMatrix;
	mCameraMatrix[3] = positionBack;
}
void Camera::TurnRight(Real degree )
{
	Matrix44 rotMat;
	rotMat.SetRotationY(DegreeToRadian(degree));
	WorldPoint4 positionBack = mCameraMatrix[3];
	mCameraMatrix[3] = WorldPoint3::ZERO;
	mCameraMatrix = rotMat*mCameraMatrix;
	mCameraMatrix[3] = positionBack;
}
void Camera::TiltUp()
{
	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(1));
	WorldPoint4 positionBack = mCameraMatrix[3];
	mCameraMatrix[3] = WorldPoint3::ZERO;
	mCameraMatrix = mCameraMatrix*rotMat;
	mCameraMatrix[3] = positionBack;
}
void Camera::TiltDown()
{

	Matrix44 rotMat;
	rotMat.SetRotationX(DegreeToRadian(-1));
	WorldPoint4 positionBack = mCameraMatrix[3];
	mCameraMatrix[3] = WorldPoint3::ZERO;
	mCameraMatrix = mCameraMatrix*rotMat;
	mCameraMatrix[3] = positionBack;
}

