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
	void TurnLeft(Real degree = -1.0);
	void TurnRight(Real degree = 1.0);
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
	Real mFov, mAspect, mNear, mFar;
};
}
