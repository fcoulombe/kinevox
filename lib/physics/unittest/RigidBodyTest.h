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

#include <gcl/UnitTest.h>
#include <gcl/AABox.h>
#include <gcl/Matrix44.h>

#include <physics/RigidBody.h>

using namespace GCL;
namespace RigidBodyTest
{

void Test()
{
	TEST_START
	PhysicsWorld::Initialize();
	{
		RigidBody box(AABox(1.,1.,1.), 10.0);
		RigidBody groundPlane(AABox(10.,1.,10.));
		Matrix44 planeTransform;
		groundPlane.GetTransform(planeTransform);
		Assert_Test(planeTransform == Matrix44::IDENTITY);
		
		Matrix44 boxTransform;
		box.GetTransform(boxTransform);
		Assert_Test(boxTransform == Matrix44::IDENTITY);
		PhysicsWorld::Update(1./60.);
		groundPlane.GetTransform(planeTransform);
		Assert_Test(planeTransform == Matrix44::IDENTITY);

		box.GetTransform(boxTransform);
		Assert_Test(boxTransform != Matrix44::IDENTITY);

		groundPlane.SetPosition(WorldPoint3(0.0, 10.0, 0.0));
		groundPlane.GetTransform(planeTransform);
		Assert_Test(planeTransform.GetPosition() == WorldPoint4(0.0, 10.0, 0.0, 1.0));

	}
	PhysicsWorld::Terminate();

}
}
