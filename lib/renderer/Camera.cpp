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

#include <iostream>
#include "renderer/Camera.h"

#include <gcl/Assert.h>
#include <gcl/Math.h>
#include "renderer/RenderPipe.h"
#include "renderer/RenderCmd.h"

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
	RenderPipe::SendCommand(new RenderCommand(RENDERER_SET_PROJECTION, &mProjectionMatrix));
	RenderPipe::SendCommand(new RenderCommand(RENDERER_SET_MODELVIEW, &mProjectionMatrix));

}


void Camera::MoveForward(Real speed)
{
	mCameraMatrix[3] += mCameraMatrix[2]/10*-speed;
}
void Camera::MoveBackward(Real speed)
{
	mCameraMatrix[3] += mCameraMatrix[2]/10*speed;
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

