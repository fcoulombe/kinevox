/*
 * Copyright (C) 2014 by Francois Coulombe
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

#include <gcl/WorldUnit.h>

namespace GCL
{
	class AABox;
	class Sphere;
	class Matrix44;
	template<typename T> class Point3;
	typedef Point3<Real> WorldPoint3;
class pRigidBody
{
public:
	pRigidBody(){}
	virtual ~pRigidBody(){}
	void *GetBody() { return mBody; }
	virtual void GetTransform(Matrix44 &mat) const = 0;
	virtual void SetPosition(const WorldPoint3 &position) = 0;
protected:
	void *mBody; // pointer to the rigid body.
};

class RigidBody
{
public:
	RigidBody(const AABox &box, Real mass = 0.0);
	RigidBody(const Sphere &sphere, Real mass = 0.0);
	~RigidBody();
	void GetTransform(Matrix44 &mat) const { mPimpl->GetTransform(mat); }
	void SetPosition(const WorldPoint3 &position)
	{
		mPimpl->SetPosition(position);
	}
private:
	friend class PhysicsWorld;
	pRigidBody *mPimpl;

};
}
