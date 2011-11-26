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

	void Update() const;


	const Matrix44 &GetTransform() const { return mMatrix; }
	static Camera &DefaultCamera();
private:
	Matrix44 mMatrix;
};
}
