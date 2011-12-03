#pragma once

#include <gcl/Matrix44.h>

namespace GCL
{
class Camera
{
public:
	Camera();
	void MoveForward();
	void MoveBackward();
	void TurnLeft(double degree = -1.0);
	void TurnRight(double degree = 1.0);
	void TiltUp();
	void TiltDown();

	void Update();


	const Matrix44 &GetTransform() const { return mCameraMatrix; }
	const Matrix44 &GetModelView() const { return mModelViewMatrix; }
	const Matrix44 &GetProjection() const { return mProjectionMatrix; }
	static Camera &DefaultCamera();
private:
	Matrix44 mCameraMatrix;
	Matrix44 mModelViewMatrix;
	Matrix44 mProjectionMatrix;
	WorldUnit mFov, mAspect, mNear, mFar;
};
}
