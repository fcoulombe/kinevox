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

#include "physics/PhysicsWorld.h"
#include "physics/RigidBody.h"
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>


using namespace GCL;

class Bullet3PhysicsWorld : public pPhysicsWorld
{
public:
	Bullet3PhysicsWorld()
	{
		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mOverlappingPairCache = new btDbvtBroadphase();
		mSolver = new btSequentialImpulseConstraintSolver;
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mOverlappingPairCache,
													 mSolver,mCollisionConfiguration);
		mDynamicsWorld->setGravity(btVector3(0,-10,0));

	}
	~Bullet3PhysicsWorld()
	{
		delete mDynamicsWorld;
		delete mSolver;
		delete mOverlappingPairCache;
		delete mDispatcher;
		delete mCollisionConfiguration;
	}

	void Update(Real dt)
	{
		mDynamicsWorld->stepSimulation(btScalar(dt),10);
	}

	void AddRigidBody(pRigidBody *body)
	{
		btRigidBody* bulletBody = (btRigidBody*)(body->GetBody());
		mDynamicsWorld->addRigidBody(bulletBody);
	}

	void RemoveRigidBody(pRigidBody *body)
	{
		btRigidBody* bulletBody = (btRigidBody*)(body->GetBody());
		mDynamicsWorld->removeCollisionObject(bulletBody);
	}
private:
	btDefaultCollisionConfiguration *mCollisionConfiguration;
	btCollisionDispatcher *mDispatcher;
	btBroadphaseInterface* mOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mSolver;
	btDiscreteDynamicsWorld* mDynamicsWorld;
};

pPhysicsWorld *PhysicsWorld::mPimpl = nullptr;
void PhysicsWorld::Initialize()
{
	mPimpl = new Bullet3PhysicsWorld();
}
void PhysicsWorld::Terminate()
{
	delete mPimpl;
	mPimpl = nullptr;
}


void PhysicsWorld::AddRigidBody(RigidBody *body)
{
	mPimpl->AddRigidBody(body->mPimpl);
}
void PhysicsWorld::RemoveRigidBody(RigidBody *body)
{
	mPimpl->RemoveRigidBody(body->mPimpl);
}

void PhysicsWorld::Update(Real dt)
{
	mPimpl->Update(dt);
}
