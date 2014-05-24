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
#include <btBulletCollisionCommon.h>

#include <BulletDynamics/Dynamics/btRigidBody.h>

//#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
//#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"


using namespace GCL;

class Bullet3RigidBody : public pRigidBody
{
public:
	Bullet3RigidBody()
	{
		btCollisionShape* shape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
		btScalar mass(0.);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			shape->calculateLocalInertia(mass,localInertia);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,-56,0));
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		mBody = new btRigidBody(rbInfo);
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
private:
	//btCollisionShape* mShape;
	//btRigidBody* mBody;
};

RigidBody::RigidBody()
{
	mPimpl = new Bullet3RigidBody();
	PhysicsWorld::AddRigidBody(this);
}

RigidBody::~RigidBody()
{
	PhysicsWorld::RemoveRigidBody(this);
	delete mPimpl;
}
