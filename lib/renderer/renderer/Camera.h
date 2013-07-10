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

#pragma once

#include <gcl/Matrix44.h>

namespace GCL
{
class Camera
{
public:
	Camera();
	void MoveForward(Real speed = 1.0);
	void MoveBackward(Real speed = 1.0);
	void TurnLeft(Real degree = -1.0);
	void TurnRight(Real degree = 1.0);
	void TiltUp();
	void TiltDown();

	//flush matrices to the renderer
	void Update();

	const Matrix44 &GetTransform() const { return mCameraMatrix; }
	Real GetFov() const { return mFov; }
	Real GetAspectRatio() const { return mAspect; }
	Real GetNear() const { return mNear; }
	Real GetFar() const { return mFar; }
	//const Matrix44 &GetModelView() const { return mModelViewMatrix; }
	//const Matrix44 &GetProjection() const { return mProjectionMatrix; }
	static Camera &DefaultCamera();
private:
	Matrix44 mCameraMatrix;
	//Matrix44 mModelViewMatrix;
	//Matrix44 mProjectionMatrix;
	Real mFov, mAspect, mNear, mFar;
};
}
