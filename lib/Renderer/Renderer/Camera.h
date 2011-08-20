#pragma once

#include <GCL/Matrix44.h>

namespace GCL
{
class Camera
{
public:
	Camera();
	void MoveForward();
	void MoveBackward();
	void TurnLeft();
	void TurnRight();
	void TiltUp();
	void TiltDown();

	void Update();


private:
	Matrix44 mMatrix;
};
}
