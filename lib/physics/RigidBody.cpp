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

#include "physics/RigidBody.h"
#include "physics/PhysicsWorld.h"
#include <gcl/AABox.h>
#include <gcl/Matrix44.h>
#include <gcl/Sphere.h>

#include <3rdparty/BulletRigidBody.h>


using namespace GCL;

class Bullet3RigidBody : public pRigidBody
{
public:
	Bullet3RigidBody(const AABox &box, Real in_mass)
	{
		btCollisionShape* shape = new btBoxShape(btVector3(btScalar(box.GetWidth()),
												 btScalar(box.GetHeight()),
												 btScalar(box.GetLength())));
		btScalar mass((btScalar)in_mass);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			shape->calculateLocalInertia(mass,localInertia);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,0,0));
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody *tempBody = new btRigidBody(rbInfo);
		tempBody->setActivationState(DISABLE_DEACTIVATION);
		mBody = tempBody;
	}
	Bullet3RigidBody(const Sphere &sphere, Real in_mass)
	{
		btCollisionShape* shape = new btSphereShape(btScalar(sphere.GetRadius()));
		btScalar mass((btScalar)in_mass);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			shape->calculateLocalInertia(mass,localInertia);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,0,0));
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody *tempBody = new btRigidBody(rbInfo);
		tempBody->setActivationState(DISABLE_DEACTIVATION);
		mBody = tempBody;
	}
	~Bullet3RigidBody()
	{
		btRigidBody* body = (btRigidBody*)mBody;
		btMotionState* motionState = body->getMotionState();
		if (motionState)
			delete motionState;
		btCollisionShape* shape = body->getCollisionShape();
		delete shape;
		delete body;
	}
	void GetTransform(Matrix44 &mat) const 
	{
		btRigidBody* body = (btRigidBody*)mBody;
		Matrix44f m;
		static_assert(sizeof(btScalar) == sizeof(float), "make sure the matrix size if the same as what bullet uses");
		body->getWorldTransform().getOpenGLMatrix((btScalar*)&m);
		mat[0] = WorldPoint4(m[0].x, m[0].y, m[0].z, m[0].w);
		mat[1] = WorldPoint4(m[1].x, m[1].y, m[1].z, m[1].w);
		mat[2] = WorldPoint4(m[2].x, m[2].y, m[2].z, m[2].w);
		mat[3] = WorldPoint4(m[3].x, m[3].y, m[3].z, m[3].w);
	}

	void SetPosition(const WorldPoint3 &position)
	{
		btRigidBody* body = (btRigidBody*)mBody;
		btTransform &t = body->getWorldTransform();
		t.setOrigin(btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z));
		body->setWorldTransform(t);
	}
private:
	//btCollisionShape* mShape;
	//btRigidBody* mBody;
};

RigidBody::RigidBody(const AABox &box, Real mass)
{
	mPimpl = new Bullet3RigidBody(box, mass);
	PhysicsWorld::AddRigidBody(this);
}
RigidBody::RigidBody(const Sphere &sphere, Real mass)
{
	mPimpl = new Bullet3RigidBody(sphere, mass);
	PhysicsWorld::AddRigidBody(this);
}

RigidBody::~RigidBody()
{
	PhysicsWorld::RemoveRigidBody(this);
	delete mPimpl;
}
